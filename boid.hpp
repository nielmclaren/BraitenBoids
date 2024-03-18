#pragma once

#include <Eigen/Dense>

using Eigen::Vector2f;

class Boid {
	static unsigned int nextId;
	unsigned int id;

	float randf();

public:
	Vector2f position;
	Vector2f velocity;

	// Remember direction even when velocity is zero.
	Vector2f direction;

	float radius;

	Boid(Vector2f pos);
	unsigned int getId();

	void step(float timeDelta);
};
