/*
 * TODO:
 * 
 * - add enabling and disabling
 */

// Copyright 2025 Caleb Whitmer

export module cakeFramework:component;

// Forward declarations of Component and Entity classes
export class Component;
export class Entity;

/**
 * @brief      the Component class is abstract an abstract interface. All
 *             Component "type" classes will define their own version of the
 *             various system calls.
 *             
 *             
 */
class Component {
	friend Entity;

   bool didAwake_ = 0;
   bool didStart_ = 0;

	// System functions
    virtual void awake(void) {}
    virtual void start(void) {}
    virtual void update(void) {}
    virtual void physicsUpdate(void) {}
    virtual void graphicsUpdate(void) {}

    virtual void onGameEnd(void) {}
    virtual void onDestroy(void) {}

 protected:
    // Pointer to the Entity instance which owns the current Component instance
    // (for use inside of Component logic)
    Entity* entity;

    const bool& didAwake = didAwake_;
    const bool& didStart = didStart_;

    // Class Entity can only access up to protected members. These are declared
    // here so that Entities can call System functions
    inline virtual void awake_(void) final { awake(); }
    inline virtual void start_(void) final { start(); }
    inline virtual void update_(void) final { update(); }
    inline virtual void physicsUpdate_(void) final { physicsUpdate(); }
    inline virtual void graphicsUpdate_(void) final { graphicsUpdate(); }

    inline virtual void onGameEnd_(void) final { onGameEnd(); }
    inline virtual void onDestroy_(void) final { onDestroy(); }

 public:
    // Virtual destructor is required for memory allocation purposes. It is
    // declared this way in order to make component and abstract class without
    // forcing subclasses from defining any specific function.
    virtual ~Component(void) = 0;
};

// Definition of pure virtual destructor to be inherited by all subclasses
Component::~Component(void) {}
