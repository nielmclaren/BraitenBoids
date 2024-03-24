#pragma once

#include <Eigen/Dense>

using Eigen::Vector2f;

class Avatar {
public:
	float radius;
	float senseRadius;
	Vector2f position;
	Vector2f direction;

	Avatar();
};

