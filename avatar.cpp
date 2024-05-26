#include "avatar.hpp"
#include <iostream>

using Eigen::Vector2f;

Avatar::Avatar() {
  numFoodsEaten = 0;

  radius = 7.f;
  sensorRange = 100.f;
}

void Avatar::handleCollision(const ICollidable &collidable) {
  numFoodsEaten++;
  // std::cout << "Avatar handle collision; numFoodsEaten=" << numFoodsEaten
  //           << std::endl;
}