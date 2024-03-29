#include <iostream>
#include <Eigen/Dense>
#include "collision_detection.hpp"

using Eigen::Vector2d;

bool CollisionDetection::detect(Avatar& avatar, FoodSource& foodSource) {
	Vector2d a(avatar.position.x(), avatar.position.y());
	Vector2d b(foodSource.position.x(), foodSource.position.y());
	float r = avatar.radius + foodSource.radius;
	return (b - a).norm() < r;
}

bool CollisionDetection::detect(Boid& boid, FoodSource& foodSource) {
	Vector2d a(boid.position.x(), boid.position.y());
	Vector2d b(foodSource.position.x(), foodSource.position.y());
	float r = boid.radius + foodSource.radius;
	return (b - a).norm() < r;
}