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
  static const float maxSpeed;

  unsigned int id;
  unsigned int generationIndex;

  unsigned int numFoodsEaten;

  std::vector<float> weights;

  NeuralNetwork neuralNetwork;

public:
  static const float radius;
  static const float senseRadius;

  Vector2f position;
  Vector2f velocity;

  // Remember direction even when velocity is zero.
  Vector2f direction;

  float speed;

  Boid(BoidProps &boidProps, Vector2f pos);
  ~Boid();

  unsigned int getId() const;
  EntityType getEntityType() const;
  AgentType getAgentType() const;

  unsigned int getGenerationIndex() const;
  int getNumFoodsEaten() const;
  std::vector<float> getWeights() const;

  float fitnessScore;

  void step(IWorldState &worldState, float timeDelta);

  void handleCollision(const ICollidable &collidable);

  BoidProps toBoidProps() const;
};
