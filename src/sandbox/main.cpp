/*
 * TODO:
 * 
 * - Start work on renderer Component
 */

// Copyright 2025 Caleb Whitmer

#include <SFML/Graphics.hpp>

import std;
import cakeFramework;

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
	// sf::RenderWindow window(sf::VideoMode({800, 600}), "cakeFramework", sf::Style::Close);

	Entity a;
	a.addComponent<Transform>()
		->setLocalPosition(sf::Vector2f{6, 9})
		->setLocalRotation(sf::Vector2f{0.7071f, 0.7071f})
		->setScale(sf::Vector2f{6, 7});

	if (auto comp = a.getComponent<Transform>())
		std::cout << comp->getLocalTransform() << std::endl;
	

	// Entity b(a);
	// b.addComponent<Transform>()->setLocalPosition(sf::Vector2f{69, 0});

	// Entity c(b);
	// c.addComponent<Transform>()->setPosition(sf::Vector2f{4, 4});

	// if (auto comp = b.getComponent<Transform>())
	// 	std::cout << comp->getTransform() << std::endl;


	Game::loop([&](){
		Game::end();
		// while (const std::optional event = window.pollEvent()) {
		// 	if (event->is<sf::Event::Closed>())
		// 		window.close();
		// }

		// if (!window.isOpen())
		// 	Game::end();
	

		// window.clear();
		// window.display();
	});

	return 0;
}
