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
const float Boid::sensorRange = 100.f;

Boid::Boid(AgentProps &props, Vector2f pos)
    : id(props.id), generationIndex(props.generationIndex), numFoodsEaten(0),
      neuralNetwork(props.weights), speed(0), pos(pos) {
  dir << 1, 0;
  Rotation2Df rotation(Util::randf(2.f * Util::pi));
  dir = rotation.toRotationMatrix() * dir;
  vel = dir * speed;
}

Boid::~Boid() {}

unsigned int Boid::getId() const { return id; }
EntityType Boid::getEntityType() const { return EntityType::Boid; }
AgentType Boid::getAgentType() const { return AgentType::Boid; }

Vector2f &Boid::position() { return pos; };
Vector2f Boid::getPosition() const { return pos; };
Vector2f &Boid::velocity() { return vel; };
Vector2f Boid::getVelocity() const { return vel; };
Vector2f &Boid::direction() { return dir; };
Vector2f Boid::getDirection() const { return dir; };
float Boid::getSpeed() const { return speed; };

std::vector<float> Boid::getWeights() const {
  return neuralNetwork.getWeights();
}

float Boid::getBoundingRadius() const { return radius; }

unsigned int Boid::getGenerationIndex() const { return generationIndex; }

int Boid::getNumFoodsEaten() const { return numFoodsEaten; }

void Boid::step(IWorldState &worldState, float timeDelta) {
  float detectionNeuron = 0;
  float directionNeuron = 0;

  std::shared_ptr<FoodSource> foodSource = worldState.getNearestFoodSource(pos);
  if (foodSource != nullptr) {
    Vector2f toFoodSource = foodSource->position() - pos;
    float dist = toFoodSource.norm();
    if (dist <= Boid::sensorRange) {
      detectionNeuron = (Boid::sensorRange - dist) / Boid::sensorRange;

      float angleBetween =
          atan2(dir.x() * toFoodSource.y() - dir.y() * toFoodSource.x(),
                toFoodSource.dot(dir));
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
  dir = rotation.toRotationMatrix() * dir;

  speed = std::clamp(
      speed + Util::linearInterp(speedNeuron, -1.f, 1.f, -0.1f, 0.1f), 0.f,
      maxSpeed);
  vel = dir * speed;

  pos += vel;
}

void Boid::handleCollision(const ICollidable &collidable) {
  numFoodsEaten++;
  // std::cout << "Boid handle collision; numFoodsEaten=" << numFoodsEaten
  //           << std::endl;
}

AgentProps Boid::toAgentProps() const {
  AgentProps props;
  props.id = getId();
  props.generationIndex = getGenerationIndex();
  props.numFoodsEaten = getNumFoodsEaten();
  props.weights = getWeights();
  return props;
}
