#include <iostream>
#include <Eigen/Dense>
#include "boid.hpp"

using Eigen::Rotation2Df;
using Eigen::Vector2f;

unsigned int Boid::nextId = 0;

Boid::Boid(Vector2f pos) {
	const float pi = std::acos(-1.0);

	id = Boid::nextId++;
	position = pos;
	direction << 1, 0;
	Rotation2Df rotation(randf() * 2 * pi);
	direction = rotation.toRotationMatrix() * direction;

	float speed = 2;
	velocity = direction * speed;

	radius = 8.f;
}

unsigned int Boid::getId() {
	return id;
}

void Boid::step(float timeDelta) {
	position += velocity;
}

float Boid::randf() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}