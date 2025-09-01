// Copyright 2025 Caleb Whitmer

#include <SFML/Graphics.hpp>

import std;
import cakeFramework;

class AComp final : public Component {
 	void start() {
		std::cout << "A start!" << std::endl;
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
	a.addComponent<AComp>();
	Entity b;


	Game::loop([&](){
		if (!window.isOpen())
			Game::end();
	
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}
	

		window.clear();
		window.display();
	});

	return 0;
}
