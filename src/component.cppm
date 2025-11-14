// Copyright 2025 Caleb Whitmer

export module cakeFramework:component;


export class Component;
export class Entity;                    // We forward declare and export Entity 
                                        // class here so that Component can 
                                        // declare Entity as a friend class.

/**
 * @ingroup  Component
 *
 * @brief    Provides an abstract
 *       interface for all
 *       Component "type" classes
 *       to inherit from. Any
 *       Component "type" class
 *       can access system
 *       functions and be added to
 *       Entity instances.
 */
class Component {
  friend Entity;

  bool didAwake_ = 0;                   // Component states.
  bool didStart_ = 0;                   // 
  bool isEnabled_ = 1;                  // 

                                        // System functions are given empty
                                        // definitions so that sub-classes are
                                        // not required to implement each-and- 
                                        // every one. Additionally, system 
                                        // functions are declared private so 
                                        // that Components cannot call system 
                                        // functions in other Components.
  virtual void awake(void)              {}
  virtual void start(void)              {}
  virtual void update(void)             {}
  virtual void physicsUpdate(void)      {}
  virtual void graphicsUpdate(void)     {}

  virtual void onDestroy(void)          {}
  virtual void onDisable(void)          {}
  virtual void onEnable(void)           {}

 protected:
  Entity* entity;                       // Pointer to the Entity instance
                                        // which owns the current Component
                                        // instance (for use inside of
                                        // Component logic).

  const bool& didAwake = didAwake_;
  const bool& didStart = didStart_;

  const bool& isEnabled = isEnabled_;

                                        // Class Entity can only access up
                                        // to protected members. These are
                                        // declared and defined here so that
                                        // Entities can call System functions.
  inline virtual void awake_(void) final          { didAwake_ = 1; awake(); }
  inline virtual void start_(void) final          { didStart_ = 1; start(); }
  inline virtual void update_(void) final         { update(); }
  inline virtual void physicsUpdate_(void) final  { physicsUpdate(); }
  inline virtual void graphicsUpdate_(void) final { graphicsUpdate(); }

  inline virtual void onDestroy_(void) final      { onDestroy(); }
  inline virtual void onDisable_(void) final      { onDisable(); }
  inline virtual void onEnable_(void) final       { onEnable(); }

 public:
  virtual ~Component(void) = 0;         // Virtual destructor is required for
                                        // memory allocation purposes. It is
                                        // declared this way in order to make
                                        // component and abstract class
                                        // without forcing sub-classes from
                                        // defining any specific function.

  /**
   * @brief    Enable the Component.
   */
  inline virtual void enable() {        //
    if (isEnabled_) return;             //
                                        //
    isEnabled_ = 1;                     //
    onEnable_();                        //
  }                                     //

  /**
   * @brief    Disable the Component.
   */
  inline virtual void disable() {       //
    if (!isEnabled_) return;            //
                                        //
    isEnabled_ = 0;                     //
    onDisable_();                       //
  }                                     //
};

Component::~Component(void) {}          // Definition of pure virtual destructor
                                        // to be inherited by all sub-classes.
