#pragma once

#include "boid.hpp"
#include "iboid_renderer.hpp"
#include "iworld_state.hpp"
#include <SFML/Graphics.hpp>

class BoidRenderer : public IBoidRenderer {
  static const std::vector<unsigned int> colors;
  static const std::vector<unsigned int> lightColors;

  sf::Transformable transform;
  sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
  const Boid &boid;
  sf::CircleShape bodyShape;
  sf::CircleShape fadeShape;
  sf::ConvexShape directionShape;
  sf::RectangleShape toNearestFoodSourceShape;

  BoidRenderer(const Boid &boid);

  unsigned int getBoidId() const;

  void draw(IWorldState &worldState, sf::RenderWindow &window);
};