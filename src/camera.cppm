// Copyright 2025 Caleb Whitmer
module;

#include<SFML/Graphics.hpp>

export module cakeFramework:camera;
import :component;
import :transform;
import :entity;

export class Camera final : public Component {
 public:
 	/**
 	 * @brief      Sets the window in which the renderer will target
 	 *
 	 * @param      window  The window
 	 *
 	 * @return     This pointer to allow function chaining
 	 */
 	Camera* setWindow(sf::RenderWindow& window) {
 		window_ = &window;
 		return this;
 	}

 private:
 	sf::RenderWindow* window_ = nullptr;
 	sf::View view_;

 	void awake() {
 		// Construct the view
 		view_ = sf::View();
 	}

	void graphicsUpdate() {
		// Break if the window has not been specified
		if (!window_)
			return;

		// Set the size of the view to the size of the window
		view_.setSize(static_cast<sf::Vector2f>(window_->getSize()));

		// If there is a transform component available
		if(auto transform = entity->getComponent<Transform>()) {
			// Then chabge the transform of the view based on that
			view_.setCenter(transform->getPosition());
			view_.setRotation(transform->getRotation().angle());
		}

		// Set the view of the window based on the new view which was calculated
		// by this componnent.
		window_->setView(view_);
	}
}; 
