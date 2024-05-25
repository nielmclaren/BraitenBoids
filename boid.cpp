#include "boid.hpp"
#include "food_source.hpp"
#include "util.hpp"
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>

using Eigen::Rotation2Df;
using Eigen::Vector2f;

const float Boid::maxSpeed = 2.f;
const float Boid::radius = 8.f;
const float Boid::senseRadius = 200.f;

Boid::Boid(AgentProps &props, Vector2f pos) : neuralNetwork(props.weights) {
  id = props.id;
  generationIndex = props.generationIndex;

  position = pos;
  direction << 1, 0;
  Rotation2Df rotation(Util::randf(2.f * Util::pi));
  direction = rotation.toRotationMatrix() * direction;

  speed = 0;
  velocity = direction * speed;

  numFoodsEaten = 0;
}

Boid::~Boid() {}

unsigned int Boid::getId() const { return id; }
EntityType Boid::getEntityType() const { return EntityType::Boid; }
AgentType Boid::getAgentType() const { return AgentType::Boid; }

std::vector<float> Boid::getWeights() const {
  return neuralNetwork.getWeights();
}

unsigned int Boid::getGenerationIndex() const { return generationIndex; }

int Boid::getNumFoodsEaten() const { return numFoodsEaten; }

void Boid::step(IWorldState &worldState, float timeDelta) {
  float detectionNeuron = 0;
  float directionNeuron = 0;

  std::shared_ptr<FoodSource> foodSource =
      worldState.getNearestFoodSource(position);
  if (foodSource != nullptr) {
    Vector2f toFoodSource = foodSource->position - position;
    float dist = toFoodSource.norm();
    if (dist <= Boid::senseRadius) {
      detectionNeuron = (Boid::senseRadius - dist) / Boid::senseRadius;

      float angleBetween = atan2(direction.x() * toFoodSource.y() -
                                     direction.y() * toFoodSource.x(),
                                 toFoodSource.dot(direction));
      directionNeuron = angleBetween / Util::pi;
    }
  }

  std::vector input({detectionNeuron, directionNeuron});
  std::vector output = neuralNetwork.forward(input);
  float speedNeuron = output[0];
  float turnNeuron = output[1];

  float maxTurnDegrees = 1;
  float turnAmount = std::min(abs(turnNeuron), maxTurnDegrees * Util::pi / 180);
  float turn =
      turnAmount == 0 ? turnAmount : turnAmount * turnNeuron / abs(turnNeuron);
  Rotation2Df rotation(turn);
  direction = rotation.toRotationMatrix() * direction;

  speed = std::clamp(
      speed + Util::linearInterp(speedNeuron, -1.f, 1.f, -0.1f, 0.1f), 0.f,
      maxSpeed);
  velocity = direction * speed;

  position += velocity;
}

void Boid::handleCollision(const ICollidable &collidable) {
  numFoodsEaten++;
  // std::cout << "Boid handle collision; numFoodsEaten=" << numFoodsEaten <<
  // std::endl;
}

AgentProps Boid::toAgentProps() const {
  AgentProps props;
  props.id = getId();
  props.generationIndex = getGenerationIndex();
  props.numFoodsEaten = getNumFoodsEaten();
  props.weights = getWeights();
  return props;
}
