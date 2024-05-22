#pragma once

#include "ientity.hpp"

class IEntityListener {
public:
  virtual ~IEntityListener() {}
  virtual void entityCreated(IEntity &entity) = 0;
  virtual void entityDeleted(IEntity &entity) = 0;
};