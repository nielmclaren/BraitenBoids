#pragma once

class ICollidable {
public:
  virtual ~ICollidable() {}
  virtual void handleCollision(ICollidable *collidable) = 0;
};