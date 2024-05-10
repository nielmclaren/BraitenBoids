#pragma once

#include "avatar.hpp"
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>

using Eigen::Rotation2Df;
using Eigen::Vector2f;

class AvatarRenderer {
  const float pi = std::acos(-1.0f);

  sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
  Avatar *avatar;
  sf::CircleShape *bodyShape;
  sf::RectangleShape *toNearestFoodSourceShape;
  sf::RectangleShape *directionShape;
  sf::RectangleShape *newDirectionShape;

  AvatarRenderer(Avatar *avatar);
  ~AvatarRenderer();

  void draw(sf::RenderWindow &window);
};