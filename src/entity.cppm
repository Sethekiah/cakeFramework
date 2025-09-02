// Copyright 2025 Caleb Whitmer
export module cakeFramework:entity;
import :component;

import std;

/**
 * @brief      Entity instances are empty by default. Developers can add
 *             Components to the Entity to provide functionality.
 *             
 *             
 */
class Entity final {
    friend class Internal;
    friend class Entity_Root_Manager;

    friend void destroy(Entity& instance);

    inline static Entity* root_ = nullptr;

    Entity* parent_ = nullptr;
    std::unordered_set<Entity*> children_;

    std::unordered_map<std::type_index, Component*> components_;

    bool isEnabled_ = 1;

    /**
     * @brief      Sets the parent of the current instance.
     *
     * @param      parent  The parent
     */
    void setParent_(Entity* parent) {
        // Insert this into the new parent's children set
        parent->children_.insert(this);

        // Set the new parent
        parent_ = parent;
    }

    /**
     * @brief      Update system - called once per frame
     */
    void update_(void) {
        // Do not call system if Entity is disabled
        if (!isEnabled_)
            return;

        for (auto& it : components_) {
            // Do not call system if Component is disabled
            if (!it.second->isEnabled)
                continue;

            // If the Start call of the current Component has not been called
            // yet, call it
            if (!(it.second->didStart_)) {
                it.second->start_();
                it.second->didStart_ = 1;
            }

            it.second->update_();
        }

        for (auto& it : children_) {
            it->update_();
        }
    }

    /**
     * @brief      Physics Update system - called 60 times per second
     */
    void physicsUpdate_(void) {
        // Do not call system if Entity is disabled
        if (!isEnabled_)
            return;

        for (auto& it : components_) {
            // Do not call system if Component is disabled
            if (!it.second->isEnabled)
                continue;

            it.second->physicsUpdate_();
        }

        for (auto& it : children_) {
            it->physicsUpdate_();
        }
    }

    /**
     * @brief      Graphics Update system - called once per frame - specifically
     *             meant for draw calls
     */
    void graphicsUpdate_(void) {
        // Do not call system if Entity is disabled
        if (!isEnabled_)
            return;

        for (auto& it : components_) {
            // Do not call system if Component is disabled
            if (!it.second->isEnabled)
                continue;

            it.second->graphicsUpdate_();
        }

        for (auto& it : children_) {
            it->graphicsUpdate_();
        }
    }

    /**
     * @brief      onGameEnd system - called only once when the game is ended
     */
    void onGameEnd_(void) {
        // Do not call system if Entity is disabled
        if (!isEnabled_)
            return;

        for (auto& it : components_) {
            // Do not call system if Component is disabled
            if (!it.second->isEnabled)
                continue;

            it.second->onGameEnd_();
        }

        for (auto& it : children_) {
            it->onGameEnd_();
        }
    }

    /**
     * @brief      onDestroy system - called when an Entity is destroyed
     */
    void onDestroy_(void) {
        // Do not call system if Entity is disabled
        if (!isEnabled_)
            return;

        for (auto& it : components_) {
            // Do not call system if Component is disabled
            if (!it.second->isEnabled)
                continue;

            it.second->onDestroy_();
        }

        for (auto& it : children_) {
            it->onDestroy_();
        }
    }

    /**
     * @brief      Disable any Components that aren't already disabled - called
     *             when an Entity is disabled
     */
    void onEntityDisable_(void) {
        for (auto& it : components_) {
            // Do not call system if Component is already disabled
            if (!it.second->isEnabled)
                continue;

            it.second->onDisable_();
        }

        for (auto& it : children_) {
            it->onEntityDisable_();
        }
    }

    /**
     * @brief      Re-enable only the Components which were previously enabled -
     *             called when an entity is enabled
     */
    void onEntityEnable_(void) {
        for (auto& it : components_) {
            // Do not call system if Component was previously disabled
            if (!it.second->isEnabled)
                continue;

            it.second->onEnable_();
        }

        for (auto& it : children_) {
            it->onEntityEnable_();
        }
    }

 public:
    const bool& isEnabled = isEnabled_;

    /**
     * @brief      Enable the Entity.
     */
    inline virtual void enable() {
      if (isEnabled_)
         return;

      isEnabled_ = 1;
      onEntityEnable_();
    }

    /**
     * @brief      Disable the Entity.
     */
    inline virtual void disable() {
      if (!isEnabled_)
         return;

      isEnabled_ = 0;
      onEntityDisable_();
    }

    /**
     * @brief      Constructs a new instance without a parent (root node)
     */
    Entity(void) {
        // Finish here if this is the root entity is being set
        if (nullptr == root_)
            return;

        // Otherwise set the parent of the current instance to be the root
        // Entity
        setParent_(root_);
    }

    /**
     * @brief      Constructs a new instance being assigned as a child of
     *             another
     *
     * @param      parent  The parent which will "adopt" the new instance
     */
    Entity(Entity& parent) {
        setParent_(&parent);
    }

    /**
     * @brief      Destroys the object and deallocates all memory stored within
     */
    ~Entity(void) {
        removeAllComponents();
    }

    /**
     * @brief      Get the parent of the current instance
     *
     * @return     The parent if it exists, null otherwise
     */
    Entity* getParent(void) {
        return parent_;
    }

    /**
     * @brief      Delete all memory allocations stored in the Components map.
     */
    void removeAllComponents() {
        for (auto it = components_.begin(); it != components_.end(); it++) {
            delete it->second;
            it->second = nullptr;
        }
        components_.clear();
    }

    /**
     * @brief      Adds a component to the current instance
     *
     * @tparam     T     The component type
     *
     * @return     A pointer to the component if it was added, null otherwise
     */
    template<class T>
    T* addComponent(void) {
        // Emplace a new component using its type as the key
        auto insertionState = components_.emplace(std::make_pair(
            static_cast<std::type_index>(typeid(T)), new T()));

        // If the component already existed then return nullptr
        if (!insertionState.second)
            return nullptr;

        // Set the Entity pointer of the Component to point to this instance
        insertionState.first->second->entity = this;

        // Call the awake function of the Component
        insertionState.first->second->awake_();
        insertionState.first->second->didAwake_ = 1;

        // Otherwise return a pointer to the new component
        return static_cast<T*>(insertionState.first->second);
    }

    /**
     * @brief      Removes a specified component from the current instance
     *
     * @tparam     T     The component type
     */
    template<class T>
    void removeComponent(void) {
        // Find the location of the component in the map using the typeid as the
        // index
        auto itToComponent = components_.find(typeid(T));

        // If the component does not exist then return
        if (itToComponent == components_.end())
            return;

        // Delete the memory allocation for the component
        delete itToComponent->second;
        itToComponent->second = nullptr;

        // Remove the pair from the map
        components_.erase(itToComponent);
    }

    /**
     * @brief      Gets a specified Component from the current instance
     *
     * @tparam     T     The Component type
     *
     * @return     A pointer to the component if it exists, null otherwise
     */
    template<class T>
    T* getComponent(void) {
        // Find the location of the component in the map using the typeid as the
        // index
        auto itToComponent = components_.find(typeid(T));

        // If the component does not exist then return null
        if (itToComponent == components_.end())
            return nullptr;

        // Otherwise return the component
        return static_cast<T*>(itToComponent->second);
    }

    /**
     * @brief      Gets a specified Component from the current instance's
     *             parent.
     *
     * @tparam     T     The Component type
     *
     * @return     A pointer to the component if it exists, null otherwise
     */
    template<class T>
    T* getComponentInParent(void) {
        // If the current instance does not have a parent then return a null
        // pointer
        if (parent_ == nullptr)
            return nullptr;

        // Otherwise get the desired Component in the instance's parent
        return parent_->getComponent<T>();
    }

    /**
     * @brief      Gets all of the Components of a specific type in children of
     *             the current instance.
     *
     * @tparam     T     The Component type
     *
     * @return     The an optional object containing a vector of pointers to the
     *             desired Component type in the children of the current
     *             instance.
     */
    template<class T>
    std::optional<std::vector<T*>> getComponentsInChildren(void) {
        // Create a vector to store pointers to all of the Components which are
        // found in the children of the current instance
        std::vector<T*> componentVector;

        // Fill the vector with any and all Components found
        std::for_each(children_.begin(), children_.end(), [&](auto& it){
            if (auto* comp = it-> template getComponent<T>())
                componentVector.push_back(comp);
        });

        // Return a null pointer if no Components where found, otherwise return
        // a pointer to an a vector storing pointers to each Component
        return (componentVector.size() == 0)
                ? std::nullopt
                : std::make_optional<std::vector<T*>>(
                    std::move(componentVector));
    }
};

/**
 * @brief      Destroys the given instance.
 *
 * @param      instance  The instance
 */
void destroy(Entity& instance) {
    // Call the onDestroy function before the object is destroyed
    instance.onDestroy_();

    // Erase the object from the Entity tree. All child objects should follow (I
    // think).
    instance.parent_->children_.erase(&instance);
}

/**
 * @brief      This class exists only to manage the memory allocated for the
 *             static root Entity instance.
 */
class Entity_Root_Manager {
 public:
    Entity_Root_Manager() {
        Entity::root_ = new Entity();
    }

    ~Entity_Root_Manager(void) {
        delete Entity::root_;
    }
} instance__;
