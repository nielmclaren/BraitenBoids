#pragma once

#include "constants.hpp"
#include "icollidable.hpp"
#include "ientity.hpp"
#include <Eigen/Dense>

using Eigen::Vector2f;

class FoodSource : public IEntity, public ICollidable {
  static unsigned int nextId;
  unsigned int id;

public:
  Vector2f position;
  float radius;

  FoodSource(Vector2f pos);
  unsigned int getId() const;
  EntityType getEntityType() const;

  void handleCollision(const ICollidable &collidable);
};