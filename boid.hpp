#pragma once
#include "boid_props.hpp"
#include "iagent.hpp"
#include "icollidable.hpp"
#include "iworld_state.hpp"
#include "neural_network.hpp"
#include <Eigen/Dense>
#include <cereal/access.hpp>
#include <cereal/cereal.hpp>

using Eigen::Vector2f;

class Boid : public IAgent, public ICollidable {
  friend class cereal::access;

  const float MAX_SPEED = 2;

  unsigned int id;
  unsigned int generationIndex;

  unsigned int numFoodsEaten;

  std::vector<float> weights;

  NeuralNetwork neuralNetwork;

  template <class Archive> void serialize(Archive &archive) {
    archive(CEREAL_NVP(id), CEREAL_NVP(generationIndex),
            CEREAL_NVP(numFoodsEaten), CEREAL_NVP(neuralNetwork));
  }

public:
  Vector2f position;
  Vector2f velocity;

  // Remember direction even when velocity is zero.
  Vector2f direction;

  float speed;

  float radius;
  float senseRadius;

  Boid();
  Boid(BoidProps &boidProps, Vector2f pos);
  ~Boid();

  unsigned int getId() const;

  unsigned int getGenerationIndex();
  int getNumFoodsEaten();
  std::vector<float> getWeights();

  float fitnessScore;

  void step(IWorldState &worldState, float timeDelta);

  void handleCollision(const ICollidable &collidable);
};
