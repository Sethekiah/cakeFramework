// Copyright 2025 Caleb Whitmer
module;

#include<SFML/Graphics.hpp>

export module cakeFramework:camera;
import :component;
import :transform;
import :entity;

/**
 * @ingroup    Component
 *
 * @brief      Defines a view-port for
 *             the selected window based
 *             on the transform of the
 *             master Entity instance if
 *             the transform exists.
 */
export class Camera final : public Component {
 public:
  /**
   * @brief      Sets the window which
   *             the renderer will
   *             target
   *
   * @param      window  The window
   *
   * @return     This pointer to allow
   *             function chaining
   */
  Camera*                               //
  setWindow(sf::RenderWindow& window) { //
    window_ = &window;                  // Set the render window and return
    return this;                        // 'this' to allow for function
                                        // chaining.
  }                                     //

 private:
  sf::RenderWindow* window_ = nullptr;
  sf::View view_;

  void awake() {                        //
    view_ = sf::View();                 // Construct the view.
  }                                     //
                                        //
  void graphicsUpdate() {               //
    if (!window_) return;               // Break if the window has not been
                                        // specified.
                                        //
    view_.setSize(                      // Set the size of the view to the size
      static_cast<sf::Vector2f>(window_->getSize()));
                                        // of the window.
                                        //
    if(auto transform =                 // If there is a transform Component 
      entity->getComponent<Transform>()) {
      view_.setCenter(                  // available, then change the transform 
        transform->getPosition());      // of the view based on the Transform of
      view_.setRotation(                // the Entity.
        transform->getRotation().angle());
    }                                   //
                                        //
    window_->setView(view_);            // Set the view of the window based
                                        // on the new view which was
                                        // calculated by this Component.
  }
}; 
