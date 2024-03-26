#pragma once

#include "avatar.hpp"
#include "boid.hpp"
#include "food_source.hpp"

class CollisionDetection {
public:
	static bool detect(Avatar& avatar, FoodSource& foodSource);
	static bool detect(Boid& boid, FoodSource& foodSource);
};

