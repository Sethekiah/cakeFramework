/*
 * TODO:
 * 
 * - Start work on transform and renderer Components
 */

// Copyright 2025 Caleb Whitmer

#include <SFML/Graphics.hpp>

import std;
import cakeFramework;

class AComp final : public Component {
 public:
 	std::string name = "";

 private:
 	void start() {
		std::cout << name << std::endl;
	}

	void update() {
	}

	void physicsUpdate() {

	}

	void graphicsUpdate() {

	}

	void onGameEnd() {
		std::cout << "Bye Bye!" << std::endl;
	}

	void onDestroy() {
		std::cout << "Killed!" << std::endl;
	}
};

int main(int argc, char const *argv[])
{
	// sf::RenderWindow window(sf::VideoMode({800, 600}), "cakeFramework", sf::Style::Close);

	Entity a;
	a.addComponent<AComp>()->name = "Caleb";	

	Entity b(a);
	b.addComponent<AComp>()->name = "eee";

	// a.removeAllComponents();

	Game::loop([&](){
		destroy(b);
		Game::end();
		// if (!window.isOpen())
		// 	Game::end();
	
		// while (const std::optional event = window.pollEvent()) {
		// 	if (event->is<sf::Event::Closed>())
		// 		window.close();
		// }
	

		// window.clear();
		// window.display();
	});

	return 0;
}
