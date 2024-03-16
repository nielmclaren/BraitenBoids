#pragma once

#include <Eigen/Dense>

using Eigen::Vector2f;

class Boid {
	static unsigned int nextId;
	unsigned int id;

public:
	Vector2f position;
	float radius;

	Boid(Vector2f pos);
	unsigned int getId();

	void step(float timeDelta);
};
