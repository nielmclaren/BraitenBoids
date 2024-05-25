#pragma once

#include "food_source.hpp"
#include <SFML/Graphics.hpp>

class FoodSourceRenderer {
  sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
  const FoodSource &foodSource;
  sf::CircleShape shape;

  FoodSourceRenderer(const FoodSource &foodSource);

  void draw(sf::RenderWindow &window);
};