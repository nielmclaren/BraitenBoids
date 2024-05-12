#pragma once

#include "food_source.hpp"
#include <SFML/Graphics.hpp>

class FoodSourceRenderer {
  const float pi = std::acos(-1.0f);

  sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
  FoodSource &foodSource;
  sf::CircleShape shape;

  FoodSourceRenderer(FoodSource &foodSource);

  void draw(sf::RenderWindow &window);
};