#pragma once

#include "boid.hpp"
#include <SFML/Graphics.hpp>

class BoidRenderer {
  const float pi = std::acos(-1.0f);

  static const std::vector<unsigned int> palette;

  sf::Transformable transform;
  sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
  const Boid &boid;
  sf::CircleShape bodyShape;
  sf::ConvexShape directionShape;
  sf::RectangleShape toNearestFoodSourceShape;

  BoidRenderer(const Boid &boid);

  void draw(sf::RenderWindow &window);
};