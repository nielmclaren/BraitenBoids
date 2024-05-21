#pragma once

#include "iworld_state.hpp"

class IAgent {
public:
  virtual ~IAgent() {}
  virtual unsigned int getId() const = 0;
  virtual void step(IWorldState &worldState, float timeDelta) = 0;
};