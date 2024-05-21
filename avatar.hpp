#pragma once

#include "icollidable.hpp"
#include <Eigen/Dense>

using Eigen::Vector2f;

class Avatar : public ICollidable {
  unsigned int numFoodsEaten;

public:
  float radius;
  float senseRadius;
  Vector2f position;
  Vector2f direction;

  Avatar();

  void handleCollision(const ICollidable &collidable);
};
