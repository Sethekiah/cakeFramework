/*
 * TODO:
 * 
 * - Start work on renderer Component
 */

// Copyright 2025 Caleb Whitmer
#include <SFML/Graphics.hpp>

import std;
import cakeFramework;
import testComp;
import spriteRenderer;

std::ostream& operator <<(std::ostream& os, const sf::Transform obj) {
	auto matrix = obj.getMatrix();
	for (int i = 0; i < 16; i++) {
		os << matrix[i] << " ";
		if (i && (i + 1) % 4 == 0)
			os << "\n";
	}
	return os;
}

std::ostream& operator <<(std::ostream& os, const sf::Vector2f obj) {
	os << obj.x << ", " << obj.y;
	return os;
}

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode({800, 600}), "cakeFramework", sf::Style::Close);

	Entity a;
	a.addComponent<SpriteRenderer>()
		->setWindow(window);
	a.addComponent<Transform>()
		->setLocalRotation({0.5f, 0.866f})
		->setLocalPosition({200, 200});
	a.addComponent<TestComp>();

	Entity b(a);
	b.addComponent<SpriteRenderer>()
		->setWindow(window);
	b.addComponent<Transform>()
		->setLocalScale({0.25, 0.25})
		->setLocalPosition({125, 0});


	Game::loop([&](){
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		if (!window.isOpen())
			Game::end();
	

		// Display the buffer first, then clear it. Render draw calls are made
		// when the graphics update system is run at the start of each game
		// loop.
		window.display();
		window.clear();
	});

	return 0;
}
