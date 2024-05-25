#pragma once
#include "agent_props.hpp"
#include "constants.hpp"
#include "iagent.hpp"
#include "icollidable.hpp"
#include "iworld_state.hpp"
#include "neural_network.hpp"
#include <Eigen/Dense>

using Eigen::Vector2f;

class Boid : public IAgent {
  static const float maxSpeed;

  unsigned int id;
  unsigned int generationIndex;

  unsigned int numFoodsEaten;

  NeuralNetwork neuralNetwork;
  std::vector<float> weights;

  float speed;
  Vector2f pos;
  Vector2f velocity;

  // Remember direction even when velocity is zero.
  Vector2f direction;

public:
  static const float radius;
  static const float senseRadius;

  Boid(AgentProps &agentProps, Vector2f position);
  ~Boid();

  unsigned int getId() const;
  EntityType getEntityType() const;
  Vector2f &position();
  Vector2f getPosition() const;
  Vector2f getDirection() const;
  float getSpeed() const;

  AgentType getAgentType() const;

  unsigned int getGenerationIndex() const;
  int getNumFoodsEaten() const;
  std::vector<float> getWeights() const;

  float getBoundingRadius() const;

  void step(IWorldState &worldState, float timeDelta);
  void handleCollision(const ICollidable &collidable);

  AgentProps toAgentProps() const;
};
