
#pragma once

#include "constants.hpp"
#include <Eigen/Dense>

using Eigen::Vector2f;

// Objects in the simulation world, such as food sources or boids.
class IEntity {
public:
  virtual ~IEntity() {}
  virtual unsigned int getId() const = 0;
  virtual EntityType getEntityType() const = 0;
  virtual Vector2f &position() = 0;
  virtual Vector2f getPosition() const = 0;
};
