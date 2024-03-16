#pragma once

#include <Eigen/Dense>

using Eigen::Vector2f;

class FoodSource {
	static unsigned int nextId;
	unsigned int id;

public:
	Vector2f position;
	float radius;

	FoodSource(Vector2f pos);
	unsigned int getId();
};