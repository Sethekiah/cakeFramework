// Copyright 2025 Caleb Whitmer
module;

#include<SFML/Graphics.hpp>

export module testComp;
import cakeFramework;
import std;

export class TestComp final : public Component {
	float angle = 0;

	void update() {
		std::cout << 1.f/Time::deltaTime << std::endl;
	}

	void physicsUpdate() {
		angle += (30.f/60.f);

		if (auto transform = entity->getComponent<Transform>())
			transform
				->setLocalRotation(sf::Vector2f{1.f, sf::degrees(angle)});
	}
};