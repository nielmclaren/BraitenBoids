#pragma once

#include "avatar.hpp"
#include "iworld_state.hpp"
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>

using Eigen::Vector2f;

class AvatarRenderer {
  sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
  const Avatar &avatar;
  sf::CircleShape bodyShape;
  sf::RectangleShape toNearestFoodSourceShape;
  sf::RectangleShape directionShape;
  sf::RectangleShape newDirectionShape;

  AvatarRenderer(const Avatar &avatar);

  void draw(IWorldState &worldState, sf::RenderWindow &window);
};