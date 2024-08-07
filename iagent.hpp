#pragma once

#include "agent_props.hpp"
#include "constants.hpp"
#include "ientity.hpp"
#include "iworld_state.hpp"


// Actors in the simulation which can perform actions during a step.
class IAgent : public ICollidable, public IEntity {
public:
  virtual ~IAgent() {}
  virtual void step(IWorldState &worldState, float timeDelta) = 0;
  virtual AgentType getAgentType() const = 0;
  virtual bool isDead() const = 0;

  virtual float getBoundingRadius() const = 0;

  virtual AgentProps toAgentProps() const = 0;
};