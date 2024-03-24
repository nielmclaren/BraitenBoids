#pragma once

#include <Eigen/Dense>

using Eigen::Vector2f;

class Simulation;
class Boid {
	const float pi = std::acos(-1.0f);

	static unsigned int nextId;
	unsigned int id;

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
};
