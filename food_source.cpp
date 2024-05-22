#include "food_source.hpp"
#include <iostream>

using Eigen::Vector2f;

unsigned int FoodSource::nextId = 0;

FoodSource::FoodSource(Vector2f pos) {
  id = FoodSource::nextId++;
  position = pos;

  radius = 5.f;
}

unsigned int FoodSource::getId() const { return id; }
EntityType FoodSource::getEntityType() const { return EntityType::FoodSource; }

void FoodSource::handleCollision(const ICollidable &collidable) {
  // std::cout << "Food source handle collision." << std::endl;
}