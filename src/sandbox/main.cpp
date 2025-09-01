// Copyright 2025 Caleb Whitmer

#include <SFML/Graphics.hpp>

import std;
import cakeFramework;

class AComp final : public Component {
 public:
 	std::string name = "";

 	void test() {
 		std::cout << name << std::endl;
 	}

 private:
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
	// sf::RenderWindow window(sf::VideoMode({800, 600}), "cakeFramework", sf::Style::Close);


	Entity a;
	
	Entity b(a);
	b.addComponent<AComp>()->name = "b";
	
	Entity c(a);
	c.addComponent<AComp>()->name = "c";

	Entity d(a);
	d.addComponent<AComp>()->name = "d";

	destroy(b);

	if (auto f = a.getComponentsInChildren<AComp>()) {
		for (auto& i : *f)
			i->test();
		// std::cout << "Yes!" << std::endl;
	}
	
	// if (auto f = b.getComponent<AComp>())
	// 	f->test();



	// Game::loop([&](){
	// 	Game::end();

	// 	if (!window.isOpen())
	// 		Game::end();
	
	// 	while (const std::optional event = window.pollEvent()) {
	// 		if (event->is<sf::Event::Closed>())
	// 			window.close();
	// 	}
	

	// 	window.clear();
	// 	window.display();
	// });

	return 0;
}
