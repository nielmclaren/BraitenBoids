#pragma once

#include "boid_props.hpp"
#include "icollidable.hpp"
#include "neural_network.hpp"
#include <Eigen/Dense>

using Eigen::Vector2f;

class Simulation;
class Boid : public ICollidable {
  const float pi = std::acos(-1.0f);
  const float MAX_SPEED = 2;

  unsigned int id;
  unsigned int generationIndex;

  unsigned int numFoodsEaten;

  NeuralNetwork neuralNetwork;

  float linearInterp(float v, float inLow, float inHigh, float outLow,
                     float outHigh);
  float randf();

public:
  Simulation *simulation;

  Vector2f position;
  Vector2f velocity;

  // Remember direction even when velocity is zero.
  Vector2f direction;

  float speed;

  float radius;
  float senseRadius;

  Boid(Simulation *sim, BoidProps &boidProps, Vector2f pos);
  unsigned int getId();

  int getGenerationIndex();
  int getNumFoodsEaten();
  std::vector<float> getWeights();

  float fitnessScore;

  void step(float timeDelta);

  void handleCollision(const ICollidable &collidable);
};
