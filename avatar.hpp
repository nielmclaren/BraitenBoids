#pragma once

#include <Eigen/Dense>

using Eigen::Vector2f;

class Simulation;
class Avatar {
public:
	Simulation* simulation;
	float radius;
	float senseRadius;
	Vector2f position;
	Vector2f direction;

	Avatar(Simulation* simulation);
};

