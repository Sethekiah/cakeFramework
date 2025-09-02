/*
 * TODO:
 * 
 * - Start work on transform and renderer Components
 */

// Copyright 2025 Caleb Whitmer

#include <SFML/Graphics.hpp>

import std;
import cakeFramework;

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode({800, 600}), "cakeFramework", sf::Style::Close);

	Entity a;

	Game::loop([&](){
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		if (!window.isOpen())
			Game::end();
	

		window.clear();
		window.display();
	});

	return 0;
}
