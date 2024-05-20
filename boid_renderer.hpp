#pragma once

#include "boid.hpp"
#include <SFML/Graphics.hpp>

class BoidRenderer {
  static const std::vector<unsigned int> colors;
  static const std::vector<unsigned int> lightColors;

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