#pragma once

class ICollidable {
public:
  virtual ~ICollidable() {}
  virtual void handleCollision(const ICollidable &collidable) = 0;
};