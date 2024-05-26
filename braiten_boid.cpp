#include "braiten_boid.hpp"
#include "food_source.hpp"
#include "util.hpp"
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>

using Eigen::Rotation2Df;
using Eigen::Vector2f;

const float BraitenBoid::maxSpeed = 2.f;
const float BraitenBoid::radius = 8.f;
const float BraitenBoid::sensorRange = 100.f;

BraitenBoid::BraitenBoid(AgentProps &props, Vector2f pos)
    : id(props.id), generationIndex(props.generationIndex), numFoodsEaten(0),
      neuralNetwork(props.weights), speed(0), pos(pos) {
  dir << 1, 0;
  Rotation2Df rotation(Util::randf(2.f * Util::pi));
  dir = rotation.toRotationMatrix() * dir;
  vel = dir * speed;
}

BraitenBoid::~BraitenBoid() {}

unsigned int BraitenBoid::getId() const { return id; }
EntityType BraitenBoid::getEntityType() const {
  return EntityType::BraitenBoid;
}
AgentType BraitenBoid::getAgentType() const { return AgentType::BraitenBoid; }

Vector2f &BraitenBoid::position() { return pos; };
Vector2f BraitenBoid::getPosition() const { return pos; };
Vector2f &BraitenBoid::velocity() { return vel; };
Vector2f BraitenBoid::getVelocity() const { return vel; };
Vector2f &BraitenBoid::direction() { return dir; };
Vector2f BraitenBoid::getDirection() const { return dir; };
float BraitenBoid::getSpeed() const { return speed; };

std::vector<float> BraitenBoid::getWeights() const {
  return neuralNetwork.getWeights();
}

float BraitenBoid::getBoundingRadius() const { return radius; }

unsigned int BraitenBoid::getGenerationIndex() const { return generationIndex; }

int BraitenBoid::getNumFoodsEaten() const { return numFoodsEaten; }

void BraitenBoid::step(IWorldState &worldState, float timeDelta) {
  float detectionNeuron = 0;
  float directionNeuron = 0;

  std::shared_ptr<FoodSource> foodSource = worldState.getNearestFoodSource(pos);
  if (foodSource != nullptr) {
    Vector2f toFoodSource = foodSource->position() - pos;
    float dist = toFoodSource.norm();
    if (dist <= BraitenBoid::sensorRange) {
      detectionNeuron =
          (BraitenBoid::sensorRange - dist) / BraitenBoid::sensorRange;

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

void BraitenBoid::handleCollision(const ICollidable &collidable) {
  numFoodsEaten++;
  // std::cout << "BraitenBoid handle collision; numFoodsEaten=" <<
  // numFoodsEaten
  //           << std::endl;
}

AgentProps BraitenBoid::toAgentProps() const {
  AgentProps props;
  props.id = getId();
  props.generationIndex = getGenerationIndex();
  props.numFoodsEaten = getNumFoodsEaten();
  props.weights = getWeights();
  return props;
}
