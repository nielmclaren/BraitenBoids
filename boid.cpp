#include "boid.hpp"
#include "food_source.hpp"
#include "simulation.hpp"
#include "util.hpp"
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>

using Eigen::Rotation2Df;
using Eigen::Vector2f;

Boid::Boid(Simulation *sim, BoidProps &props, Vector2f pos)
    : neuralNetwork(props.weights) {
  id = props.id;
  generationIndex = props.generationIndex;

  simulation = sim;
  position = pos;
  direction << 1, 0;
  Rotation2Df rotation(Util::randf(2.f * pi));
  direction = rotation.toRotationMatrix() * direction;

  speed = 0;
  velocity = direction * speed;

  numFoodsEaten = 0;

  radius = 8.f;
  senseRadius = 200.f;
}

unsigned int Boid::getId() const { return id; }

std::vector<float> Boid::getWeights() { return neuralNetwork.getWeights(); }

unsigned int Boid::getGenerationIndex() { return generationIndex; }

int Boid::getNumFoodsEaten() { return numFoodsEaten; }

void Boid::step(float timeDelta) {
  float detectionNeuron = 0;
  float directionNeuron = 0;

  FoodSource *foodSource = simulation->getNearestFoodSource(position);
  if (foodSource != nullptr) {
    Vector2f toFoodSource = foodSource->position - position;
    float dist = toFoodSource.norm();
    if (dist <= senseRadius) {
      detectionNeuron = (senseRadius - dist) / senseRadius;

      float angleBetween = atan2(direction.x() * toFoodSource.y() -
                                     direction.y() * toFoodSource.x(),
                                 toFoodSource.dot(direction));
      directionNeuron = angleBetween / pi;
    }
  }

  std::vector input({detectionNeuron, directionNeuron});
  std::vector output = neuralNetwork.forward(input);
  float speedNeuron = output[0];
  float turnNeuron = output[1];

  float maxTurnDegrees = 1;
  float turnAmount = std::min(abs(turnNeuron), maxTurnDegrees * pi / 180);
  float turn =
      turnAmount == 0 ? turnAmount : turnAmount * turnNeuron / abs(turnNeuron);
  Rotation2Df rotation(turn);
  direction = rotation.toRotationMatrix() * direction;

  speed = std::clamp(speed + linearInterp(speedNeuron, -1.f, 1.f, -0.1f, 0.1f),
                     0.f, MAX_SPEED);
  velocity = direction * speed;

  position += velocity;
}

void Boid::handleCollision(const ICollidable &collidable) {
  numFoodsEaten++;
  // std::cout << "Boid handle collision; numFoodsEaten=" << numFoodsEaten <<
  // std::endl;
}

float Boid::linearInterp(float v, float inLow, float inHigh, float outLow,
                         float outHigh) {
  return outLow + (v - inLow) * (outHigh - outLow) / (inHigh - inLow);
}
