// Copyright 2025 Caleb Whitmer
export module cakeFramework:component;

// Forward declarations of Component and Entity classes
export class Component;
export class Entity;

/**
 * \ingroup Component
 * @brief      Provides an abstract interface for all Component "type" classes
 *             to inherit from. Any Compnent "type" class can access system
 *             functions and be added to Entity instances.
 */
class Component {
	friend Entity;

   bool didAwake_ = 0;
   bool didStart_ = 0;

   bool isEnabled_ = 1;

	// System functions
    virtual void awake(void) {}
    virtual void start(void) {}
    virtual void update(void) {}
    virtual void physicsUpdate(void) {}
    virtual void graphicsUpdate(void) {}

    virtual void onGameEnd(void) {}
    virtual void onDestroy(void) {}
    virtual void onDisable(void) {}
    virtual void onEnable(void) {}

 protected:
    // Pointer to the Entity instance which owns the current Component instance
    // (for use inside of Component logic)
    Entity* entity;

    const bool& didAwake = didAwake_;
    const bool& didStart = didStart_;

    const bool& isEnabled = isEnabled_;

    // Class Entity can only access up to protected members. These are declared
    // here so that Entities can call System functions
    inline virtual void awake_(void) final { awake(); }
    inline virtual void start_(void) final { start(); }
    inline virtual void update_(void) final { update(); }
    inline virtual void physicsUpdate_(void) final { physicsUpdate(); }
    inline virtual void graphicsUpdate_(void) final { graphicsUpdate(); }

    inline virtual void onGameEnd_(void) final { onGameEnd(); }
    inline virtual void onDestroy_(void) final { onDestroy(); }
    inline virtual void onDisable_(void) final { onDisable(); }
    inline virtual void onEnable_(void) final { onEnable(); }

 public:
    // Virtual destructor is required for memory allocation purposes. It is
    // declared this way in order to make component and abstract class without
    // forcing subclasses from defining any specific function.
    virtual ~Component(void) = 0;

    /**
     * @brief      Enable the Component.
     */
    inline virtual void enable() {
      if (isEnabled_)
         return;

      isEnabled_ = 1;
      onEnable_();
    }

    /**
     * @brief      Disable the Component.
     */
    inline virtual void disable() {
      if (!isEnabled_)
         return;

      isEnabled_ = 0;
      onDisable_();
    }
};

// Definition of pure virtual destructor to be inherited by all subclasses
Component::~Component(void) {}
