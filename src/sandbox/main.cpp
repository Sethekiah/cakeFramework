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

int main(int argc, char const *argv[])
{
  sf::RenderWindow window(sf::VideoMode({800, 600}), "cakeFramework", sf::Style::Close);

  Entity a;
  a.addComponent<SpriteRenderer>()
    ->setWindow(window);
  a.addComponent<Transform>()
    ->setLocalPosition({0, 0});

  Entity b(a);
  b.addComponent<Camera>()
    ->setWindow(window);
  b.addComponent<Transform>()
    ->setLocalPosition({0, 0});
  b.addComponent<TestComp>();


  for (;;) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    if (!window.isOpen()) break;
  
    Entity::updateAll();

    window.display();                   // Display the buffer first, then
                                        // clear it. Render draw calls are
                                        // made when the graphics update
                                        // system is run at the start of
                                        // each game loop.
    window.clear();
  }

  return 0;
}