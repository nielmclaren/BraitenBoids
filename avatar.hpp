#pragma once

#include <Eigen/Dense>
#include "icollidable.hpp"

using Eigen::Vector2f;

class Simulation;
class Avatar : public ICollidable {
	unsigned int numFoodsEaten;

public:
	Simulation* simulation;
	float radius;
	float senseRadius;
	Vector2f position;
	Vector2f direction;

	Avatar(Simulation* simulation);

	void handleCollision(ICollidable* collidable);
};

