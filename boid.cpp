#include <iostream>
#include <Eigen/Dense>
#include "food_source.hpp"
#include "simulation.hpp"
#include "boid.hpp"

using Eigen::Rotation2Df;
using Eigen::Vector2f;

unsigned int Boid::nextId = 0;

Boid::Boid(Simulation* sim, Vector2f pos) {
	id = Boid::nextId++;
	simulation = sim;
	position = pos;
	direction << 1, 0;
	Rotation2Df rotation(randf() * 2 * pi);
	direction = rotation.toRotationMatrix() * direction;

	float speed = 2;
	velocity = direction * speed;

	radius = 8.f;
	senseRadius = 200.f;
}

unsigned int Boid::getId() {
	return id;
}

void Boid::step(float timeDelta) {
	FoodSource* foodSource = simulation->getNearestFoodSource(position);
	if (foodSource != nullptr) {
		Vector2f toFoodSource = foodSource->position - position;
		float dist = (toFoodSource).norm();
		if (dist <= senseRadius) {
			float angleBetween = atan2(direction.x() * toFoodSource.y() - direction.y() * toFoodSource.x(), toFoodSource.dot(direction));
			Rotation2Df rotation(angleBetween * 0.01f);
			direction = rotation.toRotationMatrix() * direction;

			float speed = 2;
			velocity = direction * speed;
		}
	}

	position += velocity;
}

float Boid::randf() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}