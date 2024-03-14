#pragma once

#include "avatar.hpp"
#include "food_source.hpp"

class CollisionDetection {
public:
	static bool detect(Avatar &avatar, FoodSource& foodSource);
};

