// Copyright 2025 Caleb Whitmer
module;

#include <SFML/Graphics.hpp>

export module cakeFramework:time;

// Forward declaration of Time class
export class Time;

class Time {
	inline static float deltaTime_ = 0;
	inline static float elapsedTime_ = 0;
	inline static float physicsDeltaTime_ = 0;
	inline constexpr static const float physicsRate = 1.f/60.f;
	inline static bool phyiscsFlag_ = 0;

 	static void refresh_(void) {
 		static sf::Clock deltaClock;
		static float physicsTimer = 0;

 		// Update delta time
 		deltaTime_ = deltaClock.restart().asSeconds();

 		// Use delta time to increment and update elapsed time
 		elapsedTime_ += deltaTime_;

 		// Use delta time to update and increment physics delta time
		phyiscsFlag_ = 0;
		physicsTimer += deltaTime_;
 		if (physicsTimer >= physicsRate) {
 			physicsDeltaTime_ = physicsTimer;
 			physicsTimer = 0;
 			phyiscsFlag_ = 1;
 		}
 	}

 public:
	friend void refresh__(void);

 	inline constexpr static const float& deltaTime = deltaTime_;
 	inline constexpr static const float& elapsedTime = elapsedTime_;
 	inline constexpr static const float& physicsDeltaTime = physicsDeltaTime_;

};