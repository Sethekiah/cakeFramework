// Copyright 2025 Caleb Whitmer
module;

#include<SFML/Graphics.hpp>

export module cakeFramework:transform;
import :component;
import :entity;

import std;

/**
 * @private
 *
 * @brief      Swap the components of a
 *             vector
 *
 * @param[in]  vector  The vector
 *
 * @return     The vector with the x & y
 *             components swapped
 */
sf::Vector2f ComponentSwap__(const sf::Vector2f& vector) {
  float yVal = vector.y;                //
  return sf::Vector2f{                  //
    yVal,                               //
    vector.x                            //
  };                                    //
}                                       //

/**
 * @private
 *
 * @brief      Internal struct used to
 *             represent and manipulate
 *             transform data. It
 *             essentially works as an
 *             unrolled affine matrix,
 *             where scale, rotation,
 *             and position are
 *             separated.
 */
struct TransformData__ {
  sf::Vector2f position  {0, 0};
  sf::Vector2f rotation  {1, 0};
  sf::Vector2f scale     {1, 1};

  /**
   * @brief      Gets a SFML transform
   *             object representing the
   *             transform stored within
   *             this struct
   *
   * @return     The SFML transform
   *             object
   */
  sf::Transform getSFMLTransform() {
    static sf::Transformable out;
    out.setPosition(position);
    out.setRotation(rotation.angle());
    out.setScale(scale);
    return out.getTransform();
  }

  /**
   * @brief      Gets the inverse of the
   *             current transform data
   *             instance.
   *
   * @return     The inverse.
   */
  TransformData__ getInverse() {        //
    static const float epislon = 0.001f;//
                                        //
    TransformData__ out;                //
                                        //
    out.rotation =                      // Invert the rotation.
      rotation.componentWiseMul({1, -1});
                                        //
    out.scale = sf::Vector2f{           // Invert the components of the scale if
      (scale.x >= epislon)              // they are not equal to zero.
        ? 1.f / scale.x : scale.x,      //
      (scale.y >= epislon)              //
        ? 1.f / scale.y : scale.y       //
    };                                  //
                                        //
    sf::Vector2f scaledPos = (out.scale.componentWiseMul(-1.f * position));
    out.position = sf::Vector2f{        // Get the scaled position and rotate it
      out.rotation                      // by the inverted rotation to find the
         .componentWiseMul({1, -1})     // inverted position.
         .dot(scaledPos),               //
      ComponentSwap__(out.rotation)     //
        .dot(scaledPos)                 //
    };                                  //
                                        //
    return out;                         //
  }                                     //

  /**
   * @brief      Combine the transforms
   *             of two TransformData
   *             structs
   *
   * @param[in]  lhs   The left hand 
   *                   side
   * @param[in]  rhs   The right hand 
   *                   side
   *
   * @return     The result of the 
   *             multiplication
   */
  friend TransformData__ operator*(TransformData__ lhs, const TransformData__& rhs) {
    sf::Vector2f scaledPos =            // Scale the lhs position by the rhs 
      lhs.scale                         // scale.
         .componentWiseMul(rhs.position);
                                        //
    lhs.position += sf::Vector2f{       // Rotate the scaled lhs position and
      lhs.rotation                      // add that value to the original.     
         .componentWiseMul({1, -1})     //                            
         .dot(scaledPos),               //                  
      ComponentSwap__(lhs.rotation)     //                            
        .dot(scaledPos)                 //                
    };                                  //
                                        //
    lhs.rotation = sf::Vector2f{        // Combine the rotations of the lhs and
      lhs.rotation                      // rhs.
         .componentWiseMul({1, -1})     //
         .dot(rhs.rotation),            //
      -lhs.rotation                     //
          .componentWiseMul({1, -1})    //
          .dot(rhs.rotation.perpendicular())
    };                                  //
                                        //
    lhs.scale =                         // Combine the scales of the lhs and
      lhs.scale                         // rhs.
         .componentWiseMul(rhs.scale);  //
                                        //
    return lhs;                         // Return the updated lhs.
  }                                     //
};

/**
 * @ingroup    Component
 *
 * @brief      Defines the position,
 *             rotation, and scale of
 *             the master Entity
 *             instance
 */
export class Transform final : public Component {
 public:
  sf::Vector2f getOrigin(void) = delete;// Delete origin functions because I do
  void setOrigin(sf::Vector2f) = delete;// not like them.

  /**
   * @brief      Gets the local 
   *             transform
   *
   * @return     The local transform
   */
  inline sf::Transform getLocalTransform(void) {
    return localTransform_              //
      .getSFMLTransform();              //
  }                                     //

  /**
   * @brief      Gets the inverse of the
   *             local transform
   *
   * @return     The inverse of the
   *             local transform
   */
  inline sf::Transform getInverseLocalTransform(void) {
    return localTransform_              //
      .getInverse()                     //
      .getSFMLTransform();              //
  }                                     //

  /**
   * @brief      Sets the local position
   *
   * @param[in]  position  The position
   */
  inline Transform* setLocalPosition(const sf::Vector2f& position) {
    localTransform_.position = position;//
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Gets the local position
   *
   * @return     The local position
   */
  inline sf::Vector2f getLocalPosition(void) {
    return localTransform_.position;    //
  }                                     //

  /**
   * @brief      Sets the local scale
   *
   * @param[in]  scale  The scale
   */
  inline Transform* setLocalScale(const sf::Vector2f& scale) {
    localTransform_.scale = scale;      //
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Gets the local scale
   *
   * @return     The local scale
   */
  inline sf::Vector2f getLocalScale(void) {
    return localTransform_.scale;
  }

  /**
   * @brief      Sets the local rotation
   *
   * @param[in]  rotation  The rotation
   */
  inline Transform* setLocalRotation(const sf::Vector2f& rotation) {
    localTransform_.rotation = rotation;//
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Gets the local rotation
   *
   * @return     The local rotation
   */
  inline sf::Vector2f getLocalRotation(void) {
    return localTransform_.rotation;    //
  }                                     //

  /**
   * @brief      Gets the global
   *             transform
   *
   * @return     The global transform
   */
  inline sf::Transform getTransform(void) {
    return getGlobalTransformData_()    //
      .getSFMLTransform();              //
  }                                     //

  /**
   * @brief      Gets the inverse of the
   *             global transform
   *
   * @return     The inverse of the
   *             global transform
   */
  inline sf::Transform getInverseTransform(void) {
    return getGlobalTransformData_()    //
      .getInverse().getSFMLTransform(); //
  }                                     //

  /**
   * @brief      Sets the global
   *             position
   *
   * @param[in]  position  The global
   *                       position
   */
  Transform* setPosition(const sf::Vector2f& position) {
    TransformData__ parentTransform =   // Get the parent transform and use it
      getParentGlobalTransformData_();  // to calculate the desired transform.
    TransformData__ desiredTransform =  //
      (parentTransform * localTransform_);
                                        //
    desiredTransform.position =         // Set the desired data of the desired
      position;                         // transform.
                                        //
    localTransform_ =                   // Calculate the new local transform to
      parentTransform.getInverse() *    // evaluate to the desired global
      desiredTransform;                 // transform.
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Gets the global
   *             position
   *
   * @return     The global position
   */
  inline sf::Vector2f getPosition(void) {
    return getGlobalTransformData_()    //
      .position;                        //
  }                                     //

  /**
   * @brief      Sets the global scale
   *
   * @param[in]  scale  The global scale
   */
  Transform* setScale(const sf::Vector2f& scale) {
    TransformData__ parentTransform =   // Get the parent transform and use it
      getParentGlobalTransformData_();  // to calculate the desired transform.
    TransformData__ desiredTransform =  //
      (parentTransform * localTransform_);
                                        //
    desiredTransform.scale = scale;     // Set the desired data of the desired
                                        // transform.
                                        //
    localTransform_ =                   // Calculate the new local transform to
      parentTransform.getInverse() *    // evaluate to the desired global
      desiredTransform;                 // transform.
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Gets the global scale
   *
   * @return     The global scale
   */
  inline sf::Vector2f getScale(void) {  //
    return getGlobalTransformData_()    //
      .scale;                           //
  }                                     //

  /**
   * @brief      Sets the global
   *             rotation
   *
   * @param[in]  rotation  The global
   *                       rotation
   */
  Transform* setRotation(const sf::Vector2f& rotation) {
    TransformData__ parentTransform =   // Get the parent transform and use it
      getParentGlobalTransformData_();  // to calculate the desired transform.
    TransformData__ desiredTransform =  //
      (parentTransform * localTransform_);
                                        //
    desiredTransform.rotation =         // Set the desired data of the desired
      rotation;                         // transform.
                                        //
    localTransform_ =                   // Calculate the new local transform to
      parentTransform.getInverse() *    // evaluate to the desired global
      desiredTransform;                 // transform.
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Gets the global
   *             rotation
   *
   * @return     The global rotation
   */
  inline sf::Vector2f getRotation(void) {
    return getGlobalTransformData_()    //
      .rotation;                        //
  }                                     //

  /**
   * @brief      Move the Transform by
   *             some offset
   *
   * @param[in]  offset  The offset
   *
   * @return     This pointer to allow
   *             function chaining
   */
  Transform* localTranslate(const sf::Vector2f& offset) {
    localTransform_.position += offset; //
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Rotate the Transform by
   *             some angle
   *
   * @param[in]  angle  The angle
   *
   * @return     This pointer to allow
   *             function chaining
   */
  Transform* localRotate(const sf::Vector2f& angle) {
    localTransform_.rotation =          //
      sf::Vector2f{                     //
        localTransform_                 //
          .rotation                     //
          .componentWiseMul({1, -1})    //
          .dot(angle),                  //
        -localTransform_                //
          .rotation                     //
          .componentWiseMul({1, -1})    //
          .dot(angle.perpendicular())   //
    };                                  //
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Scale the transform in
   *             global space by some
   *             scale multiplier
   *
   * @param[in]  scale  The scale
   *
   * @return     This pointer to allow
   *             function chaining
   */
  Transform* localScale(const sf::Vector2f& scale) {
    localTransform_.scale =             //
      localTransform_                   //
        .scale                          //
        .componentWiseMul(scale);       //
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Move the Transform in
   *             global space by some
   *             offset
   *
   * @param[in]  offset  The offset
   *
   * @return     This pointer to allow
   *             function chaining
   */
  Transform* translate(const sf::Vector2f& offset) {
    TransformData__ parentTransform =   // Get the parent transform and use it
      getParentGlobalTransformData_();  // to calculate the desired transform.
    TransformData__ desiredTransform =  //
      (parentTransform * localTransform_);
                                        //
    desiredTransform.position += offset;// Set the desired data of the desired
                                        // transform.
                                        //
    localTransform_ =                   // Calculate the new local transform to
      parentTransform.getInverse() *    // evaluate to the desired global
      desiredTransform;                 // transform.
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Rotate the Transform in
   *             global space by some
   *             angle
   *
   * @param[in]  angle  The angle
   *
   * @return     This pointer to allow
   *             function chaining
   */
  Transform* rotate(const sf::Vector2f& angle) {
    TransformData__ parentTransform =   // Get the parent transform and use it
      getParentGlobalTransformData_();  // to calculate the desired transform.
    TransformData__ desiredTransform =  //
      (parentTransform * localTransform_);
                                        //
    desiredTransform.rotation =         // Set the desired data of the desired
      sf::Vector2f{                     // transform.
        desiredTransform                //
          .rotation                     //
          .componentWiseMul({1, -1})    //
          .dot(angle),                  //
        -desiredTransform               //
          .rotation                     //
          .componentWiseMul({1, -1})    //
          .dot(angle.perpendicular())   //
    };                                  //
                                        //
    localTransform_ =                   // Calculate the new local transform to
      parentTransform.getInverse() *    // evaluate to the desired global
      desiredTransform;                 // transform.
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Scale the transform in
   *             global space by some
   *             scale multiplier
   *
   * @param[in]  scale  The scale
   *                    multiplier
   *
   * @return     This pointer to allow
   *             function chaining
   */
  Transform* scale(const sf::Vector2f& scale) {
    TransformData__ parentTransform =   // Get the parent transform and use it
      getParentGlobalTransformData_();  // to calculate the desired transform.
    TransformData__ desiredTransform =  //
      (parentTransform * localTransform_);
                                        //
    desiredTransform.scale =            // Set the desired data of the desired
      desiredTransform                  // transform.
        .scale                          //
        .componentWiseMul(scale);       //
                                        //
    localTransform_ =                   // Calculate the new local transform to
      parentTransform.getInverse() *    // evaluate to the desired global
      desiredTransform;                 // transform.
                                        //
    wasChanged_ = true;                 // Raise the transform dirty flag.
                                        //
    return this;                        //
  }                                     //

  /**
   * @brief      Get the forward vector
   *
   * @return     The vector pointing in
   *             the same direction as
   *             the transform is facing
   */
  inline sf::Vector2f forward(void) {   //
    return getGlobalTransformData_()    //
      .rotation;                        //
  }                                     //

  /**
   * @brief      Get the right vector
   *
   * @return     The vector pointing
   *             perpendicular to the
   *             direction which the
   *             transform is facing
   */
  inline sf::Vector2f right(void) {     //
    return getGlobalTransformData_()    //
      .rotation                         //
      .perpendicular();                 //
  }                                     //

 private:
   TransformData__ localTransform_;
   bool wasChanged_ = true;
   TransformData__ cachedGlobalTransform_;

  /**
   * @brief      Determines whether the
   *             Entity, specified by
   *             the pointer, contains
   *             a transform
   *             (implicitly or
   *             explicitly) which is
   *             dirty
   *
   * @param      ptr   The pointer to
   *                   the Entity
   *                   instance
   *
   * @return     True if the specified
   *             pointer is dirty,
   *             False otherwise
   */
  bool isDirty_(Entity* ptr) {
    if (!ptr) return false;             // If we have reached the head Entity 
                                        // then return false.
                                        //
    if (auto transform = ptr->getComponent<Transform>())
      if (transform->wasChanged_)       // Otherwise if the current Entity has a
        return true;                    //  dirty pointer then return true.
                                        //
    return isDirty_(ptr->getParent());  // If the current Entity does not have a
                                        // transform that is dirty, then check
                                        // its parent.
  }

  /**
   * @brief      Gets the global
   *             transform data by
   *             recursively combining
   *             the current local
   *             transform with the
   *             global transforms of
   *             all parents and
   *             grandparents and so on
   *
   * @return     The global transform
   *             data
   */
  TransformData__ getGlobalTransformData_() {
    if(!isDirty_(entity))               // If the current Transform is not
      return cachedGlobalTransform_;    // dirty, then simply return our cached
                                        // value.
                                        // 
    cachedGlobalTransform_ =            // Otherwise recalculate and cache the
      [this]() -> TransformData__ {     // current global transform.
                                        //
        auto parent = this              // Get the parent of the current Entity.
          ->entity->getParent();        //
                                        //
        for(;;) {                       //
          if (!parent)                  // If the current Entity has no parent
            return localTransform_;     // then return its local matrix.
                                        //
                                        // If the parent has a Transform
                                        // Component then recursively return it
                                        // multiplied by the current local
                                        // matrix.
          if(auto pTransform = parent->getComponent<Transform>())
            return pTransform->getGlobalTransformData_() * localTransform_;
                                        //
          parent = parent->getParent(); // Otherwise get the grandparent.
        }                               //
      }();                              //
                                        //
    wasChanged_ = false;                // Update the changed flag.
                                        //
    return cachedGlobalTransform_;      // Return the newly cached global 
                                        // transform.
  }                                     //

  /**
   * @brief      Gets the global
   *             transform of the parent
   *             of the current entity
   *
   * @return     The parent global
   *             transform data
   */
  TransformData__ getParentGlobalTransformData_() {
    TransformData__ parentGlobalTransform;
                                        //
                                        // Get the global transform of the
                                        // parent or alternatively a grandparent
                                        // if they exist. Otherwise, return the
                                        // identity transform.
    auto parent = entity->getParent();  //
    if (parent) {                       //
      for(;;) {                         //
        if (auto pTransform = parent->getComponent<Transform>()) {
          parentGlobalTransform = pTransform->getGlobalTransformData_();
          break;                        //
        }                               //
                                        //
        parent = parent->getParent();   //
                                        //
        if (!parent) break;             //
      }                                 //
    }                                   //
                                        //
    return parentGlobalTransform;       //
  }                                     //
};
