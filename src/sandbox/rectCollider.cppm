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

  float outlineThickness = 1.f;

 private:
  const float size_ = 100.f;            // Default size of the collider, will
                                        // change dynamically.
                                        // 
  const sf::Vector2f unit_ = {1, 1};    // Unit vector for multiplication
                                        //
  sf::RectangleShape shape_;            //
                                        //
  void awake() {                        //
    shape_.setSize(size_ * unit_);      // Set default size and origin.
    shape_.setOrigin(size_ * .5f * unit_);
                                        //
    shape_.setOutlineColor(             // Set outline color.
      displayColor);                    //
                                        //
    shape_.setFillColor(                // Make inside transparent so that it
      sf::Color::Transparent);          // outlines the Entity.
                                        //
    shape_.setOutlineThickness(         // Finally set the thickness of the
      outlineThickness);                // outline.
  }                                     //
                                        //
  void update() {                       // Since we are going off of AABB 
                                        // colliders, the collider should 
                                        // circumscribe the transform.
    if (auto transform = entity->getComponent<Transform>()) {
      sf::Vector2f v1 =                 //
        transform->getRotation();       //
      v1 = sf::Vector2f{std::abs(v1.x), // Create a vector based on the
                        std::abs(v1.y)};// transform.
                                        //
      sf::Vector2f v2 =                 // Create another vector, perpendicular
        v1.perpendicular();             // to the transform.
                                        //
      sf::Vector2f scale =              //
        transform->getScale();          //
      v1 *= scale.x;                    //
      v2 *= scale.y;                    // Scale each vector appropriately.
                                        //
      sf::Vector2f circ = {             // Use both vectors to get two of the 
        v1.x - v2.x,                    // corners of transform.
        v1.y + v2.y                     //
      };                                // 
                                        //
      circ *= size_;                    // Scale the vector containing the 
                                        // corner data.
                                        // 
      shape_.setSize(circ);             // Set the size and origin of the 
      shape_.setOrigin(0.5f * circ);    // collider based on this vector. This
                                        // will circumscribe the transform.
                                        //
      shape_.setPosition(               // Copy the position data of the 
        transform->getPosition());      // transform to the position of the
                                        // collider.
    }                                   //
  }                                     //
                                        //
  void graphicsUpdate() {               //
    if (!renderWindow) return;          //
                                        //
    renderWindow->draw(shape_);         //
  }                                     //
};