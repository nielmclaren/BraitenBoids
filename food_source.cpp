#include "food_source.hpp"

using Eigen::Vector2f;

unsigned int FoodSource::nextId = 0;

FoodSource::FoodSource(Vector2f pos) {
	id = FoodSource::nextId++;
	position = pos;

	radius = 5.f;
}

unsigned int FoodSource::getId() {
	return id;
}