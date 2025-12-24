// Copyright 2025 Caleb Whitmer
module;

#include<SFML/Graphics.hpp>

export module testComp;
import cakeFramework;
import std;

export class TestComp final : public Component {
	void start() {
		if (auto transform = entity->getComponent<Transform>())
			transform->setScale({4.f, .5f});
	}

	void update() {
		if (auto transform = entity->getComponent<Transform>()) {
			transform->rotate(sf::Vector2f(1.f, sf::degrees(0.005f)));	
			transform->translate(transform->forward() * 10.f * Time::deltaTime);
		}
	}

	void physicsUpdate() {

	}
};