/*
 * TODO:
 * 
 * - Figure out ways to make this code more efficient
 * 	- Dirty flags or something?
 */

// Copyright 2025 Caleb Whitmer
module;

#include<SFML/Graphics.hpp>

export module cakeFramework:transform;
import :entity;
import :component;

import std;

/**
 * @brief      Swap the components of a vector
 *
 * @param[in]  vector  The vector
 *
 * @return     The vector with the x & y components swapped
 */
sf::Vector2f ComponentSwap__(const sf::Vector2f& vector) {
	float yVal = vector.y;
	return sf::Vector2f{
		yVal,
		vector.x
	};
}

/**
 * @brief      Internal struct used to represent and manipulate transform data.
 *             It essentially works as an unrolled affine matrix, where scale,
 *             rotation, and position are separated.
 */
struct TransformData__ {
	sf::Vector2f position	{0, 0};
	sf::Vector2f rotation	{1, 0};
	sf::Vector2f scale		{1, 1};

	/**
	 * @brief      Gets a sfml transform object representing the transform
	 *             stored within this struct
	 *
	 * @return     The sfml transform object
	 */
	sf::Transform getSFMLTransform() {
		static sf::Transformable out;
		out.setPosition(position);
		out.setRotation(rotation.angle());
		out.setScale(scale);
		return out.getTransform();
	}

	/**
	 * @brief      Gets the inverse of the current transform data instance.
	 *
	 * @return     The inverse.
	 */
	TransformData__ getInverse() {
		static const float epislon = 0.001f;

		TransformData__ out;

		// Invert the rotation
		out.rotation = rotation.componentWiseMul({1, -1});

		// Invert the components of the scale if they are not equal to zero
		out.scale = sf::Vector2f{
			(scale.x >= epislon) ? 1.f / scale.x : scale.x, 
			(scale.y >= epislon) ? 1.f / scale.y : scale.y 
		};

		// Get the scaled position and rotate it by the inverted rotation to
		// find the inverted position
		sf::Vector2f scaledPos = (out.scale.componentWiseMul(-1.f * position));
		out.position = sf::Vector2f{
			out.rotation.componentWiseMul({1, -1}).dot(scaledPos),
			ComponentSwap__(out.rotation).dot(scaledPos)
		};

		return out;
	}

	/**
	 * @brief      Combine the transforms of two TransformData structs
	 *
	 * @param[in]  lhs   The left hand side
	 * @param[in]  rhs   The right hand side
	 *
	 * @return     The result of the multiplication
	 */
	friend TransformData__ operator*(TransformData__ lhs, const TransformData__& rhs) {
		// Scale the lhs position by the rhs scale
		sf::Vector2f scaledPos = lhs.scale.componentWiseMul(rhs.position);

		// Rotate the scaled lhs position and add that value to the original 
		lhs.position += sf::Vector2f{
			lhs.rotation.componentWiseMul({1, -1}).dot(scaledPos),
			ComponentSwap__(lhs.rotation).dot(scaledPos)
		};

		// Combine the rotations of the lhs and rhs
		lhs.rotation = sf::Vector2f{
			lhs.rotation.componentWiseMul({1, -1}).dot(rhs.rotation),
			-lhs.rotation.componentWiseMul({1, -1}).dot(rhs.rotation.perpendicular())
		};

		// Combine the scales of the lhs and rhs
		lhs.scale = lhs.scale.componentWiseMul(rhs.scale);

		// Return the updated lhs
		return lhs;
	}
};

export class Transform final : public Component {
 public:
 	// Delete origin functions because I do not like them
 	sf::Vector2f getOrigin(void) = delete;
 	void setOrigin(sf::Vector2f) = delete;

 	/**
 	 * @brief      Gets the local transform.
 	 *
 	 * @return     The local transform.
 	 */
 	inline sf::Transform getLocalTransform(void) {
 		return localTransform_.getSFMLTransform();
 	}

 	/**
 	 * @brief      Gets the inverse of the local transform.
 	 *
 	 * @return     The inverse of the local transform.
 	 */
 	inline sf::Transform getInverseLocalTransform(void) {
 		return localTransform_.getInverse().getSFMLTransform();
 	}

 	/**
 	 * @brief      Sets the local position.
 	 *
 	 * @param[in]  position  The position
 	 */
 	inline Transform* setLocalPosition(const sf::Vector2f& position) {
 		localTransform_.position = position;
		
		// Raise the transform dirty flag
		isDirty_ = true;
		
 		return this;
 	}

 	/**
 	 * @brief      Gets the local position.
 	 *
 	 * @return     The local position.
 	 */
 	inline sf::Vector2f getLocalPosition(void) {
 		return localTransform_.position;
 	}

 	/**
 	 * @brief      Sets the local scale.
 	 *
 	 * @param[in]  scale  The scale
 	 */
 	inline Transform* setLocalScale(const sf::Vector2f& scale) {
 		localTransform_.scale = scale;
		
		// Raise the transform dirty flag
		isDirty_ = true;
		
 		return this;
 	}

 	/**
 	 * @brief      Gets the local scale.
 	 *
 	 * @return     The local scale.
 	 */
 	inline sf::Vector2f getLocalScale(void) {
 		return localTransform_.scale;
 	}

 	/**
 	 * @brief      Sets the local rotation.
 	 *
 	 * @param[in]  rotation  The rotation
 	 */
 	inline Transform* setLocalRotation(const sf::Vector2f& rotation) {
 		localTransform_.rotation = rotation;
		
		// Raise the transform dirty flag
		isDirty_ = true;

 		return this;
 	}

 	/**
 	 * @brief      Gets the local rotation.
 	 *
 	 * @return     The local rotation.
 	 */
 	inline sf::Vector2f getLocalRotation(void) {
 		return localTransform_.rotation;
 	}

	/**
	 * @brief      Gets the global transform.
	 *
	 * @return     The global transform.
	 */
	inline sf::Transform getTransform(void) {
		return getGlobalTransformData_().getSFMLTransform();
	} 	

	/**
	 * @brief      Gets the inverse of the global transform.
	 *
	 * @return     The inverse of the global transform.
	 */
	inline sf::Transform getInverseTransform(void) {
		return getGlobalTransformData_().getInverse().getSFMLTransform();
	}

	/**
	 * @brief      Sets the global position.
	 *
	 * @param[in]  position  The global position
	 */
	Transform* setPosition(const sf::Vector2f& position) {
		// Get the parent transform and use it to calculate the desired
		// transform
		TransformData__ parentTransform = getParentGlobalTransformData_();
		TransformData__ desiredTransform = (parentTransform * localTransform_);

		// Set the desired data of the desired transform
		desiredTransform.position = position;

		// Calculate the new local transform to evaluate to the desired global
		// transform
		localTransform_ = parentTransform.getInverse() * desiredTransform;
		
		// Raise the transform dirty flag
		isDirty_ = true;

		return this;
	}

	/**
	 * @brief      Gets the global position.
	 *
	 * @return     The global position.
	 */
	inline sf::Vector2f getPosition(void) {
		return getGlobalTransformData_().position;
	}

	/**
	 * @brief      Sets the global scale.
	 *
	 * @param[in]  scale  The global scale
	 */
	Transform* setScale(const sf::Vector2f& scale) {
		// Get the parent transform and use it to calculate the desired
		// transform
		TransformData__ parentTransform = getParentGlobalTransformData_();
		TransformData__ desiredTransform = (parentTransform * localTransform_);

		// Set the desired data of the desired transform
		desiredTransform.scale = scale;

		// Calculate the new local transform to evaluate to the desired global
		// transform
		localTransform_ = parentTransform.getInverse() * desiredTransform;
		
		// Raise the transform dirty flag
		isDirty_ = true;

		return this;
	}

	/**
	 * @brief      Gets the global scale.
	 *
	 * @return     The global scale.
	 */
	inline sf::Vector2f getScale(void) {
		return getGlobalTransformData_().scale;
	}

	/**
	 * @brief      Sets the global rotation.
	 *
	 * @param[in]  rotation  The global rotation
	 */
	Transform* setRotation(const sf::Vector2f& rotation) {
		// Get the parent transform and use it to calculate the desired
		// transform
		TransformData__ parentTransform = getParentGlobalTransformData_();
		TransformData__ desiredTransform = (parentTransform * localTransform_);

		// Set the desired data of the desired transform
		desiredTransform.rotation = rotation;

		// Calculate the new local transform to evaluate to the desired global
		// transform
		localTransform_ = parentTransform.getInverse() * desiredTransform;

		// Raise the transform dirty flag
		isDirty_ = true;

		return this;
	}

	/**
	 * @brief      Gets the global rotation.
	 *
	 * @return     The global rotation.
	 */
	inline sf::Vector2f getRotation(void) {
		return getGlobalTransformData_().rotation;
	}

	/**
	 * @brief      Move the Transform by some offset
	 *
	 * @param[in]  offset  The offset
	 *
	 * @return     This pointer to allow function chaining
	 */
	Transform* translate(const sf::Vector2f& offset) {
		// Get the parent transform and use it to calculate the desired
		// transform
		TransformData__ parentTransform = getParentGlobalTransformData_();
		TransformData__ desiredTransform = (parentTransform * localTransform_);

		// Set the desired data of the desired transform
		desiredTransform.position += offset;

		// Calculate the new local transform to evaluate to the desired global
		// transform
		localTransform_ = parentTransform.getInverse() * desiredTransform;
		
		// Raise the transform dirty flag
		isDirty_ = true;

		return this;
	}

	/**
	 * @brief      Rotate the Transform by some angle
	 *
	 * @param[in]  angle  The angle
	 *
	 * @return     This pointer to allow function chaining
	 */
	Transform* rotate(const sf::Vector2f& angle) {
		// Get the parent transform and use it to calculate the desired
		// transform
		TransformData__ parentTransform = getParentGlobalTransformData_();
		TransformData__ desiredTransform = (parentTransform * localTransform_);

		// Set the desired data of the desired transform
		// desiredTransform.rotation = rotation;
		desiredTransform.rotation = sf::Vector2f{
			desiredTransform.rotation.componentWiseMul({1, -1}).dot(angle),
			-desiredTransform.rotation.componentWiseMul({1, -1})
				.dot(angle.perpendicular())
		};

		// Calculate the new local transform to evaluate to the desired global
		// transform
		localTransform_ = parentTransform.getInverse() * desiredTransform;

		// Raise the transform dirty flag
		isDirty_ = true;

		return this;
	}

	/**
	 * @brief      Scale the transform by some scale multiplier
	 *
	 * @param[in]  scale  The scale multiplier
	 *
	 * @return     This pointer to allow function chaining
	 */
	Transform* scale(const sf::Vector2f& scale) {
		// Get the parent transform and use it to calculate the desired
		// transform
		TransformData__ parentTransform = getParentGlobalTransformData_();
		TransformData__ desiredTransform = (parentTransform * localTransform_);

		// Set the desired data of the desired transform
		desiredTransform.scale = desiredTransform.scale.componentWiseMul(scale);

		// Calculate the new local transform to evaluate to the desired global
		// transform
		localTransform_ = parentTransform.getInverse() * desiredTransform;
		
		// Raise the transform dirty flag
		isDirty_ = true;

		return this;
	}

	/**
	 * @brief      Get the forward vector
	 *
	 * @return     The vector pointing in the same direction as the transform is
	 *             facing
	 */
	inline sf::Vector2f forward(void) {
		return getGlobalTransformData_().rotation;
	}

	/**
	 * @brief      Get the right vector
	 *
	 * @return     The vector pointing perpendicular to the direction which the
	 *             transform is facing
	 */
	inline sf::Vector2f right(void) {
		return getGlobalTransformData_().rotation.perpendicular();
	}

 private:
 	TransformData__ localTransform_;
 	bool isDirty_ = true;
 	TransformData__ cachedGlobalTransform_;

 	/**
 	 * @brief      Marks all closest descendants that own a Transform Component
 	 *             as dirty.
 	 *
 	 * @param      ptr   The pointer to the Entity whose descendants will be
 	 *                   marked as dirty
 	 */
 	void markDescendantsDirty_(Entity* ptr) {
 		if (auto children = ptr->getChildren())
 			for (auto child : *children) {
 				if (auto transform = child->getComponent<Transform>()) {
 					transform->isDirty_ = true;
 					continue;
 				}
 				markDescendantsDirty_(child);
 			}
 	}

 	/**
 	 * @brief      Gets the global transform data by recursively combining the
 	 *             current local transform with the global transforms of all
 	 *             parents and grandparents and so on.
 	 *
 	 * @return     The global transform data.
 	 */
 	TransformData__ getGlobalTransformData_() {
 		// If the transform has not been dirty, then return the cached data
 		if (!isDirty_)
 			return cachedGlobalTransform_;

 		// Otherwise mark all children transforms as dirty
 		markDescendantsDirty_(entity);

 		// Get and cache the new global transform (recursive)
 		cachedGlobalTransform_ = [this]() -> TransformData__ {
 	 		// Get the parent of the current Entity
 	 		auto parent = this->entity->getParent();

 	 		for(;;) {
 	 			// If the current Entity has no parent then return its local matrix
 	 			if (!parent)
 	 				return localTransform_;

 	 			// If the parent has a Transform Component then recursively return
 	 			// it multiplied by the current local matrix
 		 		if(auto pTransform = parent->getComponent<Transform>())
 		 			return pTransform->getGlobalTransformData_() * localTransform_;

 		 		// Otherwise get the grandparent
 	 			parent = parent->getParent();
 	 		}
 		}();
 		
 		// Lower the dirty flag
 		isDirty_ = false;

 		// Finally, return the newly cached global transform 
 		return cachedGlobalTransform_;
 	}

 	/**
 	 * @brief      Gets the global transform of the parent of the current
 	 *             entity.
 	 *
 	 * @return     The parent global transform data.
 	 */
 	TransformData__ getParentGlobalTransformData_() {
 		TransformData__ parentGlobalTransform;

 		// Get the global transform of the parent or alternatively a grandparent
 		// if they exist. Otherwise, return the local transform
 		auto parent = entity->getParent();
 		if (parent) {
	 		for(;;) {
	 			if (auto pTransform = parent->getComponent<Transform>()) {
	 				parentGlobalTransform = pTransform->getGlobalTransformData_();
	 				break;
	 			}

	 			parent = parent->getParent();

	 			if (!parent)
	 				break;
	 		}
 		}

 		return parentGlobalTransform;
 	}
};
