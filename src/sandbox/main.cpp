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
import rectCollider;

int main(int argc, char const *argv[])
{
  sf::RenderWindow window(sf::VideoMode({800, 600}), "cakeFramework", sf::Style::Close);

  Entity a;
  a.addComponent<SpriteRenderer>()
    ->setWindow(window)
  ;
  a.addComponent<Transform>()
    ->setLocalPosition({0, 0})
    // ->setLocalScale({2.f, 1.f})
  ;
  a.addComponent<TestComp>();
  a.addComponent<RectCollider>()
    ->renderWindow = &window;

  Entity camera;
  camera.addComponent<Camera>()
    ->setWindow(window);
  camera.addComponent<Transform>()
    ->setLocalPosition({0, 0});


  for (;;) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    if (!window.isOpen()) break;
  
    Entity::updateAll();

    window.display();                   // Display the buffer first, then
    window.clear();                     // clear it. Render draw calls are
                                        // made when the graphics update
                                        // system is run at the start of
                                        // each game loop.
  }

  return 0;
}