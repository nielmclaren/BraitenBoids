#pragma once
#include "boid_props.hpp"
#include "constants.hpp"
#include "iagent.hpp"
#include "icollidable.hpp"
#include "iworld_state.hpp"
#include "neural_network.hpp"
#include <Eigen/Dense>

using Eigen::Vector2f;

class Boid : public IAgent, public ICollidable {
  const float MAX_SPEED = 2;

  unsigned int id;
  unsigned int generationIndex;

  unsigned int numFoodsEaten;

  std::vector<float> weights;

  NeuralNetwork neuralNetwork;

public:
  Vector2f position;
  Vector2f velocity;

  // Remember direction even when velocity is zero.
  Vector2f direction;

  float speed;

  float radius;
  float senseRadius;

  Boid(BoidProps &boidProps, Vector2f pos);
  ~Boid();

  unsigned int getId() const;
  EntityType getEntityType() const;
  AgentType getAgentType() const;

  unsigned int getGenerationIndex();
  int getNumFoodsEaten();
  std::vector<float> getWeights();

  float fitnessScore;

  void step(IWorldState &worldState, float timeDelta);

  void handleCollision(const ICollidable &collidable);

  BoidProps toBoidProps();
};
