#include <iostream>
#include "avatar.hpp"

using Eigen::Vector2f;

Avatar::Avatar(Simulation* sim) {
	simulation = sim;

	numFoodsEaten = 0;

	radius = 7.f;
	senseRadius = 100.f;
}

void Avatar::handleCollision(ICollidable* collidable) {
	numFoodsEaten++;
	std::cout << "Avatar handle collision; numFoodsEaten=" << numFoodsEaten << std::endl;
}