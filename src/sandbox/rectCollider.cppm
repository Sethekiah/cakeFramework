// Copyright 2025 Caleb Whitmer
module;

#include<SFML/Graphics.hpp>

export module rectCollider;
import cakeFramework;
import std;

export class RectCollider final : public Component {
 public:

  sf::RenderWindow* renderWindow = 
    nullptr;
  
  sf::Color displayColor = 
    sf::Color::Magenta;

  float outlineThickness = 3.f;

 private:

  sf::RectangleShape shape_;            //
                                        //
  void awake() {                        //
    shape_.setSize({100, 100});
    shape_.setOrigin({50, 50});

    shape_.setOutlineColor(displayColor);//
                                        //
    shape_.setFillColor(                //
      sf::Color::Transparent);          //
                                        //
    shape_.setOutlineThickness(         //
      outlineThickness);                //
  }                                     //
                                        //
  void update() {                       // Since we are going off of
                                        // AABB colliders, I need to
                                        // inscribe the transform
                                        // inside the rectangle.
                                        //
  }                                     //
                                        //
  void graphicsUpdate() {               //
    if (!renderWindow) return;          //

    renderWindow->draw(shape_);
  }                                     //
};