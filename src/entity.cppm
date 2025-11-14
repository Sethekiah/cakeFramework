// Copyright 2025 Caleb Whitmer

export module cakeFramework:entity;
import :component;
import :time;

import std;

// Declaration of Entity class is
// exported in Component module.

/**
 * @ingroup    Entity
 *
 * @brief      Defines an empty object
 *             to which, Component
 *             instances can be added in
 *             order to provide
 *             functionality.
 */
class Entity final {
  friend class Entity_Root_Manager;     // Manages the memory allocation of the 
                                        // root Entity.
                                        //
  friend void destroy(Entity& instance);// Destroys an Entity instance.
                                        //
  inline static Entity* root_ = nullptr;// The location of the root Entity.
                                        //
  Entity* parent_ = nullptr;            // The parent of the current instance.
                                        //
  std::unordered_set<Entity*> children_;// Set of children of current instance.
                                        //
  std::unordered_map<std::type_index,   // Table of Components which belong to
                     Component*>        // the current instance.
    components_;                        // 
                                        //
  bool isEnabled_ = 1;                  // Enabled flag.

  /**
   * @brief      Sets the parent of the
   *             current instance.
   *
   * @param      parent  The parent
   */
  void setParent_(Entity* parent) {     //
    parent->children_.insert(this);     // Insert 'this' into the new parent's
                                        // children set.
                                        //
    parent_ = parent;                   // Set the new parent.
  }                                     //

  /**
   * @brief      Update system; called
   *             once per frame
   */
  void update_(void) {                  //
    if (!isEnabled_) return;            // Do not call system if Entity is
                                        // disabled.
                                        //
    for (auto& it : components_) {      //
      if (!it.second->isEnabled)        // Do not call system if Component is
        continue;                       // disabled.
                                        //
      if (!(it.second->didStart_))      // If the Start call of the current
        it.second->start_();            // Component has not been called
                                        // yet, call it.
                                        //
      it.second->update_();             // Finally update the Component.
    }                                   //
                                        //
    for (auto& it : children_)          // Then, update all of the children
      it->update_();                    // Entities.
  }                                     //

  /**
   * @brief      Physics Update system;
   *             called 60 times per
   *             second
   */
  void physicsUpdate_(void) {           //
    if (!isEnabled_) return;            // Do not call system if Entity is
                                        // disabled.
                                        //
    for (auto& it : components_) {      //
      if (!it.second->isEnabled)        // Do not call system if Component is
        continue;                       // disabled.
                                        //
      it.second->physicsUpdate_();      // Finally update the Component.
    }                                   //
                                        //
    for (auto& it : children_)          // Then, update all of the children
      it->physicsUpdate_();             // Entities.
  }                                     //

  /**
   * @brief      Graphics Update system;
   *             called once per frame;
   *             specifically meant for
   *             draw calls
   */
  void graphicsUpdate_(void) {          //
    if (!isEnabled_) return;            // Do not call system if Entity is
                                        // disabled.
                                        //
    for (auto& it : components_) {      //
      if (!it.second->isEnabled)        // Do not call system if Component is
        continue;                       // disabled.
                                        //
      it.second->graphicsUpdate_();     // Finally update the Component.
    }                                   //
                                        //
    for (auto& it : children_)          // Then, update all of the children
      it->graphicsUpdate_();            // Entities.
  }                                     //

  /**
   * @brief      onDestroy system;
   *             called when an Entity
   *             is destroyed
   */
  void onDestroy_(void) {               //
    if (!isEnabled_) return;            // Do not call system if Entity is
                                        // disabled.
                                        //
    for (auto& it : components_) {      //
      if (!it.second->isEnabled)        // Do not call system if Component is
        continue;                       // disabled.
                                        //
      it.second->onDestroy_();          // Finally update the Component.
    }                                   //
                                        //
    for (auto& it : children_)          // Then, update all of the children
      it->onDestroy_();                 // Entities.
  }                                     //

  /**
   * @brief      Disable any Components
   *             that aren't already
   *             disabled; called when
   *             an Entity is disabled
   */
  void onEntityDisable_(void) {         //
    for (auto& it : components_) {      //
      if (!it.second->isEnabled)        // Do not call system if Component is
        continue;                       // already disabled.
                                        //
      it.second->onDisable_();          // Finally update the Component.
    }                                   //
                                        //
    for (auto& it : children_)          // Then, update all of the children
      it->onEntityDisable_();           // Entities.
  }                                     //

  /**
   * @brief      Re-enable only the
   *             Components which were
   *             previously enabled;
   *             called when an entity
   *             is enabled
   */
  void onEntityEnable_(void) {          //
    for (auto& it : components_) {      //
      if (!it.second->isEnabled)        // Do not call system if Component was
        continue;                       // previously disabled.
                                        //
      it.second->onEnable_();           // Finally update the Component.
    }                                   //
                                        //
    for (auto& it : children_)          // Then, update all of the children
      it->onEntityEnable_();            // Entities.
  }                                     //

 public:
  const bool& isEnabled = isEnabled_;

  /**
   * @brief      Update all Entity
   *             instances, starting at
   *             the root Entity and
   *             recursively propagating
   *             through each
   *             generation.
   */
  static void updateAll(void) {         //
    Time::refresh_();                   // Refresh the game timer.
                                        //
    root_->update_();                   // Call all update functions on the root
    if (Time::phyiscsFlag_)             // instance. Updates will propagate down
      root_->physicsUpdate_();          // the Entity instance tree.
    root_->graphicsUpdate_();           //
  }                                     //

  /**
   * @brief    Enable the Entity.
   */
  inline virtual void enable(void) {    //
    if (isEnabled_) return;             //
                                        //
    isEnabled_ = 1;                     //
    onEntityEnable_();                  //
  }                                     //

  /**
   * @brief    Disable the Entity.
   */
  inline virtual void disable(void) {   //
    if (!isEnabled_) return;            //
                                        //
    isEnabled_ = 0;                     //
    onEntityDisable_();                 //
  }                                     //

  /**
   * @brief      Constructs a new
   *             instance without a
   *             parent (root node)
   */
  inline Entity(void) {                 //
    if (nullptr == root_) return;       // Finish here if this is the root
                                        // Entity is being set.
                                        // 
    setParent_(root_);                  // Otherwise set the parent of the
                                        // current instance to be the root
                                        // Entity.
  }                                     //

  /**
   * @brief      Constructs a new
   *             instance being assigned
   *             as a child of another
   *
   * @param      parent  The parent
   *                     which will
   *                     "adopt" the new
   *                     instance
   */
  Entity(Entity& parent) {              //
    setParent_(&parent);                //
  }                                     //

  /**
   * @brief      Destroys the object and
   *             de-allocates all memory
   *             stored within
   */
  ~Entity(void) {                       //
    removeAllComponents();              //
  }                                     //

  /**
   * @brief      Get the parent of the
   *             current instance
   *
   * @return     The parent if it
   *             exists, null otherwise
   */
  inline Entity* getParent(void) {      //
    return parent_;                     //
  }                                     //

  /**
   * @brief      Delete all memory
   *             allocations stored in
   *             the Components map.
   */
  void removeAllComponents(void) {      //
    for (auto it = components_.begin(); // Loop through each Component of the
         it != components_.end();       // current instance.
         it++) {                        //
                                        //
      delete it->second;                // Delete and nullify each Component.
      it->second = nullptr;             //
    }                                   //
                                        //
    components_.clear();                // Finally clear the map data structure 
  }                                     // holding holding the Components.

  /**
   * @brief      Adds a component to the
   *             current instance
   *
   * @tparam     T     The component
   *                   type
   *
   * @return     A pointer to the
   *             component if it was
   *             added, null otherwise
   */
  template<class T>                     //
  T* addComponent(void) {               //
    auto insertionState =               // Emplace a new component using its
      components_.emplace(              // type as the key.
        std::make_pair(                 //
          static_cast<std::type_index>(typeid(T)),                                
          new T()                       //
        )                               //
      );                                //
                                        //
    if (!insertionState.second)         // If the component already existed
      return nullptr;                   // then return null pointer.
                                        //
    insertionState.first                // Set the Entity pointer of the
                  ->second              // Component to point to this instance.
                  ->entity = this;      //
                                        //
    insertionState.first                // Call the awake function of the
                  ->second              // Component.
                  ->awake_();           //
                                        //
    return static_cast<T*>(             // Otherwise return a pointer to the new
      insertionState.first              // component.
                    ->second            //
    );                                  //
  }                                     //

  /**
   * @brief      Removes a specified
   *             component from the
   *             current instance
   *
   * @tparam     T     The component
   *                   type
   */
  template<class T>                     //
  void removeComponent(void) {          //
    auto itToComponent =                // Find the location of the Component in
      components_.find(typeid(T));      // the map using the type id as the
                                        // index.
                                        //
    if (itToComponent ==                // If the component does not exist then
        components_.end()) return;      // return.
                                        //
    delete itToComponent->second;       // Delete the memory allocation for the
    itToComponent->second = nullptr;    // Component.
                                        //
    components_.erase(itToComponent);   // Remove the pair from the map.
  }                                     //

  /**
   * @brief      Gets a specified
   *             Component from the
   *             current instance
   *
   * @tparam     T     The Component
   *                   type
   *
   * @return     A pointer to the
   *             component if it exists,
   *             null otherwise
   */
  template<class T>                     //
  T* getComponent(void) {               //
    auto itToComponent =                // Find the location of the Component in
      components_.find(typeid(T));      // the map using the type id as the
                                        // index.
                                        //
    if (itToComponent ==                // If the component does not exist then
        components_.end())              // return a null pointer.
      return nullptr;                   //
                                        //
    return static_cast<T*>(             // Otherwise return the Component.
      itToComponent->second             // 
    );                                  // 
  }

  /**
   * @brief      Gets a specified
   *             Component from the
   *             current instance's
   *             parent.
   *
   * @tparam     T     The Component
   *                   type
   *
   * @return     A pointer to the
   *             component if it exists,
   *             null otherwise
   */
  template<class T>                     //
  T* getComponentInParent(void) {       //
    if (nullptr == parent_)             // If the current instance does not have
      return nullptr;                   // a parent then return a null pointer.
                                        //
    return parent_->getComponent<T>();  // Otherwise get the desired Component
                                        // in the instance's parent.
  }                                     //

  /**
   * @brief      Gets all of the
   *             Components of a
   *             specific type in
   *             children of the current
   *             instance.
   *
   * @tparam     T     The Component
   *                   type
   *
   * @return     The an optional object
   *             containing a vector of
   *             pointers to the desired
   *             Component type in the
   *             children of the current
   *             instance.
   */
  template<class T>                     //
  std::optional<std::vector<T*>>        //
  getComponentsInChildren(void) {       //    
    std::vector<T*> componentVector;    // Create a vector to store pointers to
                                        // all of the Components which are found
                                        // in the children of the current
                                        // instance. 
                                        //
    std::for_each(children_.begin(),    // Loop through each child of the
                  children_.end(),      // current instance.
                  [&](auto& it){        //
                                        //
      if (auto* comp =                  // Fill the vector with any and all 
        it-> template getComponent<T>())// Components found.
        componentVector.push_back(comp);//
    });                                 //
                                        //
    return (0 == componentVector.size())// Return a null pointer if no 
      ? std::nullopt                    // Components where found, otherwise
      : std::make_optional<std::vector<T*>>(
        std::move(componentVector));    // return a pointer to a vector storing
                                        // pointers to each Component.
  }                                     //

  /**
   * @brief      Gets the children.
   *
   * @return     An optional set,
   *             containing each child
   *             of the current
   *             instance.
   */
  std::optional<std::unordered_set<Entity*>> 
  getChildren(void) {                   //
    return (0 == children_.size())      // If there are no children, then return
        ? std::nullopt                  // a null option. Otherwise, return an
        : std::make_optional<std::unordered_set<Entity*>>(
          children_);                   // optional set containing pointers to
  }                                     // each child instance.
};

/**
 * @brief      Destroys the given 
 *             instance.
 *
 * @param      instance  The instance
 */
void destroy(Entity& instance) {        //
  instance.onDestroy_();                // Call the onDestroy function before
                                        // the object is destroyed.
                                        //
  instance.parent_->                    // Erase the object from the Entity
    children_.erase(&instance);         // tree. All child objects should follow
                                        // (I think).
}                                       //

/**
 * @private
 *
 * @brief      This struct exists only
 *             to manage the memory
 *             allocated for the static
 *             root Entity instance.
 */
struct Entity_Root_Manager {
  Entity_Root_Manager() {               //
    Entity::root_ = new Entity();       // When instantiated, allocate space for
                                        // the root Entity.
  }                                     //

  ~Entity_Root_Manager(void) {          //
    delete Entity::root_;               // When destroyed, de-allocate said 
                                        // space.
  }                                     //
} instance__;                           // Create a single instance.
