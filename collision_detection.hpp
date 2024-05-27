#pragma once

#include "avatar.hpp"
#include "food_source.hpp"
#include "iagent.hpp"


class CollisionDetection {
public:
  static bool detect(Avatar &avatar, FoodSource &foodSource);
  static bool detect(IAgent &agent, FoodSource &foodSource);
};
