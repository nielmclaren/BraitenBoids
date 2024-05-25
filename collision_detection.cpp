#include "collision_detection.hpp"
#include <Eigen/Dense>
#include <iostream>

using Eigen::Vector2d;

bool CollisionDetection::detect(Avatar &avatar, FoodSource &foodSource) {
  Vector2d a(avatar.position.x(), avatar.position.y());
  Vector2d b(foodSource.position().x(), foodSource.position().y());
  float r = avatar.radius + foodSource.radius;
  return (b - a).norm() < r;
}

bool CollisionDetection::detect(IAgent &agent, FoodSource &foodSource) {
  Vector2d a(agent.position().x(), agent.position().y());
  Vector2d b(foodSource.position().x(), foodSource.position().y());
  float r = agent.getBoundingRadius() + foodSource.radius;
  return (b - a).norm() < r;
}