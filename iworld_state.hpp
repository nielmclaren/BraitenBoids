#pragma once

#include "food_source.hpp"
#include <Eigen/Dense>
#include <vector>

using Eigen::Vector2f;

class IWorldState {
public:
  virtual ~IWorldState() {}
  virtual std::shared_ptr<FoodSource> getNearestFoodSource(Vector2f &point) = 0;
  virtual float distanceToNearestFoodSource(Vector2f &point) = 0;
  virtual std::vector<std::shared_ptr<FoodSource>>
  getNearbyFoodSources(Vector2f &point, float range) = 0;
};