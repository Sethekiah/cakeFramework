/*
 * TODO:
 * 
 * - Possibly split this into two modules interface partitions
 * 
 * - Add a root singleton class to handle system calls over the architecture
 * - Add additional functions under Entity class
 *   - getComponentInParent
 *   - getComponentsInChildren
 *   - Look for more useful functions on Unity Docs
 * - Possibly add additional system functions to Component class
 * 
 */

// Copyright 2025 Caleb Whitmer
export module ecs;

import std;

// >>------------------------ Forward Declarations ------------------------<< //

export class Component;
export class Entity;

// >>----------------------------- Definitions ----------------------------<< //

/**
 * @brief      This interface class describes a Component
 */
class Component {
    friend Entity;

    // System functions
    virtual void start() {}
    virtual void update() {}
    virtual void physicsUpdate() {}
    virtual void graphicsUpdate() {}

 protected:
    // Pointer to the Entity instance which owns the current Component instance
    // (for use inside of Component logic)
    Entity* entity;

    // Class Entity can only access up to protected members. These are declared
    // here so that Entities can call System functions
    inline virtual void start_() final { start(); }
    inline virtual void update_() final { update(); }
    inline virtual void physicsUpdate_() final { physicsUpdate(); }
    inline virtual void graphicsUpdate_() final { graphicsUpdate(); }

 public:
    // Virtual destructor is required for memory allocation purposes. It is
    // declared this way in order to make component and abstract class without
    // forcing subclasses from defining any specific function.
    virtual ~Component() = 0;
};

// Definition of pure virtual destructor to be inherited by all subclasses
Component::~Component() {}

/**
 * @brief      This class describes an entity.
 */
class Entity final {
    Entity* parent_ = nullptr;
    std::unordered_set<Entity*> children_;

    std::unordered_map<std::type_index, Component*> components_;

    /**
     * @brief      Update system
     */
    void update_() {
        // Call the update function across all Components which belong to the
        // current Entity
        std::for_each(components_.begin(), components_.end(), [](auto& it){
            it.second->update_();
        });

        // Call the update function across all children of the current Entity
        std::for_each(children_.begin(), children_.end(), [](auto& it){
            it->update_();
        });
    }

    /**
     * @brief      Physics Update system
     */
    void physicsUpdate_() {
        // Call the update function across all Components which belong to the
        // current Entity
        std::for_each(components_.begin(), components_.end(), [](auto& it){
            it.second->physicsUpdate_();
        });

        // Call the update function across all children of the current Entity
        std::for_each(children_.begin(), children_.end(), [](auto& it){
            it->physicsUpdate_();
        });
    }

    /**
     * @brief      Graphics Update system
     */
    void graphicsUpdate_() {
        // Call the update function across all Components which belong to the
        // current Entity
        std::for_each(components_.begin(), components_.end(), [](auto& it){
            it.second->graphicsUpdate_();
        });

        // Call the update function across all children of the current Entity
        std::for_each(children_.begin(), children_.end(), [](auto& it){
            it->graphicsUpdate_();
        });
    }

 public:
    /**
     * @brief      Constructs a new instance without a parent
     */
    Entity(void) {}

    /**
     * @brief      Constructs a new instance being assigned as a child of
     *             another
     *
     * @param      parent  The parent which will "adopt" the new instance
     */
    Entity(Entity* parent) {
        setParent(parent);
    }

    /**
     * @brief      Destroys the object and deallocates all memory stored within
     */
    ~Entity() {
        // Delete all memory allocations stored in the Components map
        std::for_each(components_.begin(), components_.end(), [](auto& it){
            delete it.second;
            it.second = nullptr;
        });
    }

    /**
     * @brief      Set or replace the parent of the instance
     *
     * @param      parent  The new parent
     */
    void setParent(Entity* parent) {
        if (parent_ == parent or nullptr == parent)
            return;

        // Remove this from the old parent's children set
        if (parent_ != nullptr)
            parent_->children_.erase(this);

        // Insert this into the new parent's children set
        parent->children_.insert(this);

        // Set the new parent
        parent_ = parent;
    }

    /**
     * @brief      Get the parent of the current instance
     *
     * @return     The parent if it exists, null otherwise
     */
    Entity* getParent() {
        return parent_;
    }

    /**
     * @brief      Adds a component to the current instance
     *
     * @tparam     T     The component type
     *
     * @return     A pointer to the component if it was added, null otherwise
     */
    template<class T>
    T* addComponent() {
        // Emplace a new component using its type as the key
        auto insertionState = components_.emplace(std::make_pair(
            static_cast<std::type_index>(typeid(T)), new T()));

        // If the component already existed then return nullptr
        if (!insertionState.second)
            return nullptr;

        insertionState.first->second->entity = this;
        insertionState.first->second->start_();

        // Otherwise return a pointer to the new component
        return static_cast<T*>(insertionState.first->second);
    }

    /**
     * @brief      Removes a specified component from the current instance
     *
     * @tparam     T     The component type
     */
    template<class T>
    void removeComponent() {
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
     * @brief      Gets a specified component from the current instance
     *
     * @tparam     T     The component type
     *
     * @return     A pointer to the component if it exists, null otherwise
     */
    template<class T>
    T* getComponent() {
        // Find the location of the component in the map using the typeid as the
        // index
        auto itToComponent = components_.find(typeid(T));

        // If the component does not exist then return null
        if (itToComponent == components_.end())
            return nullptr;

        // Otherwise return the component
        return static_cast<T*>(itToComponent->second);
    }
};
