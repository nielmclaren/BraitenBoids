#pragma once

#include "constants.hpp"
#include "icollidable.hpp"
#include "ientity.hpp"
#include <Eigen/Dense>

using Eigen::Vector2f;

class FoodSource : public IEntity, public ICollidable {
  static unsigned int nextId;
  unsigned int id;

  Vector2f pos;

public:
  float radius;

  FoodSource(Vector2f position);
  unsigned int getId() const;
  EntityType getEntityType() const;
  Vector2f &position();
  Vector2f getPosition() const;

  void handleCollision(const ICollidable &collidable);
};