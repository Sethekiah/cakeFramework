// Copyright 2025 Caleb Whitmer
module;

#include<SFML/Graphics.hpp>

export module spriteRenderer;
import cakeFramework;
import std;

export class SpriteRenderer final : public Component {
 public:
 	/**
 	 * @brief      Sets the window in which the renderer will target
 	 *
 	 * @param      window  The window
 	 *
 	 * @return     This pointer to allow function chaining
 	 */
 	SpriteRenderer* setWindow(sf::RenderWindow& window) {
 		window_ = &window;
 		return this;
 	}

 private:
 	sf::RenderWindow* window_ = nullptr;
 	sf::RectangleShape rect_;

 	void awake() {
 		// Default size of 100px squared
 		rect_.setSize({100, 100});
 		rect_.setOrigin({50, 50});

 		// Default color of magenta (no material)
 		rect_.setFillColor(sf::Color::Red);
 	}

 	void graphicsUpdate() {
 		// Only run the graphics update if the window pointer is not null
 		if (!window_)
 			return;

 		// Get information from the Transform Component if it exists
 		sf::Transform transformOverride;
 		if (auto transform = entity->getComponent<Transform>())
 			transformOverride = transform->getTransform();

 		// Finally draw the sprite to the window
 		window_->draw(rect_, transformOverride);
 	}

};