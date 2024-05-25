#pragma once

#include "braiten_boid.hpp"
#include "iboid_renderer.hpp"
#include "iworld_state.hpp"
#include <SFML/Graphics.hpp>

class BraitenBoidRenderer : public IBoidRenderer {
  static const std::vector<unsigned int> colors;
  static const std::vector<unsigned int> lightColors;

  sf::Transformable transform;
  sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
  const BraitenBoid &boid;
  sf::CircleShape bodyShape;
  sf::ConvexShape directionShape;
  sf::RectangleShape toNearestFoodSourceShape;

  BraitenBoidRenderer(const BraitenBoid &boid);

  unsigned int getBoidId() const;
  void draw(IWorldState &worldState, sf::RenderWindow &window);
};