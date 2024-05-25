#include "food_source.hpp"
#include <iostream>

using Eigen::Vector2f;

unsigned int FoodSource::nextId = 0;

FoodSource::FoodSource(Vector2f position) : pos(position) {
  id = FoodSource::nextId++;

  radius = 5.f;
}

unsigned int FoodSource::getId() const { return id; }
EntityType FoodSource::getEntityType() const { return EntityType::FoodSource; }
Vector2f &FoodSource::position() { return pos; };
Vector2f FoodSource::getPosition() const { return pos; };

void FoodSource::handleCollision(const ICollidable &collidable) {
  // std::cout << "Food source handle collision." << std::endl;
}