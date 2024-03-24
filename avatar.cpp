#include "avatar.hpp"

using Eigen::Vector2f;

Avatar::Avatar(Simulation* sim) {
	simulation = sim;
	radius = 7.f;
	senseRadius = 100.f;
}