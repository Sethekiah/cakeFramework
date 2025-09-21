// Copyright 2025 Caleb Whitmer
module;

#include<SFML/Graphics.hpp>

export module testComp;
import cakeFramework;
import std;

export class TestComp final : public Component {
	void update() {
		// std::cout << 1.f/Time::deltaTime << std::endl;
		if (auto transform = entity->getComponent<Transform>()) {
			transform->translate(transform->forward() * Time::deltaTime * 100.f);
			transform->rotate(sf::Vector2f(1.f, sf::degrees(0.1f)));	
		}
	}

	void physicsUpdate() {

	}
};