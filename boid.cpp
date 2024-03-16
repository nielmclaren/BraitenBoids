#include <Eigen/Dense>
#include "boid.hpp"

using Eigen::Rotation2Df;
using Eigen::Vector2f;

unsigned int Boid::nextId = 0;

Boid::Boid(sf::Vector2f pos) {
	const double pi = std::acos(-1.0);

	id = Boid::nextId++;
	position = pos;

	radius = 8.f;
}

unsigned int Boid::getId() {
	return id;
}

void Boid::step(float timeDelta) {
	position.x += 5;
}
