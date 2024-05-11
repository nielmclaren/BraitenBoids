#pragma once

#include "icollidable.hpp"
#include <Eigen/Dense>

using Eigen::Vector2f;

class FoodSource : public ICollidable {
  static unsigned int nextId;
  unsigned int id;

public:
  Vector2f position;
  float radius;

  FoodSource(Vector2f pos);
  unsigned int getId();

  void handleCollision(const ICollidable &collidable);
};