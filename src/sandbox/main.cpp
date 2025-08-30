/*
 * TODO:
 * 
 * - Internal partitions
 * 
 */

// Copyright 2025 Caleb Whitmer

#include <SFML/Graphics.hpp>

import std;
import cakeFramework;

class TestComp final : public Component {
	void start() {
	}

	void update() {

	}

	void physicsUpdate() {

	}

	void graphicsUpdate() {

	}
};

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode({800, 600}), "cakeFramework", sf::Style::Close);


	Entity a;
	a.addComponent<TestComp>();
	Entity b(&a);


	Game::loop([&](){
		if (window.isOpen()) {
			while (const std::optional event = window.pollEvent()) {
				if (event->is<sf::Event::Closed>())
					window.close();
			}
		} else
			Game::end();

		window.clear();
		window.display();
	});

	return 0;
}