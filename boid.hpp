#pragma once

#include <Eigen/Dense>
#include "icollidable.hpp"

using Eigen::Vector2f;

class Simulation;
class Boid : public ICollidable {
	const float pi = std::acos(-1.0f);

	static unsigned int nextId;
	unsigned int id;

	unsigned int numFoodsEaten;

	float randf();

public:
	Simulation* simulation;

	Vector2f position;
	Vector2f velocity;

	// Remember direction even when velocity is zero.
	Vector2f direction;

	float radius;
	float senseRadius;

	Boid(Simulation *sim, Vector2f pos);
	unsigned int getId();

	void step(float timeDelta);

	void handleCollision(ICollidable* collidable);
};
