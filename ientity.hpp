
#pragma once

#include "constants.hpp"

// Objects in the simulation world, such as food sources or boids.
class IEntity {
public:
  virtual ~IEntity() {}
  virtual unsigned int getId() const = 0;
  virtual EntityType getEntityType() const = 0;
};
