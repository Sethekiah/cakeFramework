// Copyright 2025 Caleb Whitmer

module;
#include <SFML/Graphics.hpp>
export module cakeFramework:time;

/**
 * @ingroup    System
 *
 * @brief      Provides the interface
 *             for accessing time
 *             information related to
 *             the game
 */
export class Time final {
  /*! \cond PRIVATE */
  friend class Entity;
  /*! \endcond */

  inline static float deltaTime_ = 0;   //
  inline static float elapsedTime_ = 0; //
  inline static float physicsDeltaTime_ = 0;
  inline constexpr static const float physicsRate = 1.f/60.f;
  inline static bool phyiscsFlag_ = 0;  //

   /**
    * @brief      Update the game
    *             timers.
    */
  static void refresh_(void) {          // 
    static sf::Clock deltaClock;        //
    static float physicsTimer = 0;      //
                                        //
    deltaTime_ = deltaClock.restart()   // Update delta time.
                           .asSeconds();//
                                        //
    elapsedTime_ += deltaTime_;         // Use delta time to increment and
                                        // update elapsed time.
                                        // 
    phyiscsFlag_ = 0;                   // Use delta time to update and 
    physicsTimer += deltaTime_;         // increment physics delta time.
    if (physicsTimer >= physicsRate) {  //
      physicsDeltaTime_ = physicsTimer; //
      physicsTimer = 0;                 //
      phyiscsFlag_ = 1;                 //
    }                                   //
  }                                     //

 public:
                                        // Public references to private data
                                        // members are declared like so in
                                        // order to make them read only. This
                                        // removes the need for getter
                                        // members. (I hate getters).
  inline constexpr static const float& deltaTime = deltaTime_;
  inline constexpr static const float& elapsedTime = elapsedTime_;
  inline constexpr static const float& physicsDeltaTime = physicsDeltaTime_;
};
