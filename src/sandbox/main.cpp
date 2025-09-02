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
		std::cout << "Update!" << std::endl;
	}

	void physicsUpdate() {
		std::cout << "P Update!" << std::endl;
	}

	void graphicsUpdate() {
		std::cout << "G Update!" << std::endl;
	}

	void onDisable() {
		std::cout << "Disabled!" << std::endl;
	}

	void onEnable() {
		std::cout << "Enabled!" << std::endl;
	}
};

int main(int argc, char const *argv[])
{
	// sf::RenderWindow window(sf::VideoMode({800, 600}), "cakeFramework", sf::Style::Close);

	Entity a;
	a.addComponent<AComp>()->name = "a";	

	Entity b(a);
	b.addComponent<AComp>()->name = "b";
	
	a.disable();

	// Entity b(a);
	// b.addComponent<AComp>()->name = "eee";

	// a.removeAllComponents();

	Game::loop([&](){
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
