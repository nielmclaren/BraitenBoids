#pragma once

#include "boid.hpp"
#include <SFML/Graphics.hpp>

class BoidRenderer {
  const float pi = std::acos(-1.0f);

  sf::Transformable transform;
  sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
  Boid *boid;
  sf::CircleShape *bodyShape;
  sf::ConvexShape *directionShape;
  sf::RectangleShape *toNearestFoodSourceShape;

  BoidRenderer(Boid *boid);
  ~BoidRenderer();

  void draw(sf::RenderWindow &window);
};