#include "braiten_boid.hpp"
#include "food_source.hpp"
#include "util.hpp"
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>

using Eigen::Matrix2f;
using Eigen::Matrix3f;
using Eigen::Rotation2Df;
using Eigen::Vector2f;

const float BraitenBoid::maxSpeed = 2.f;
const float BraitenBoid::radius = 8.f;
const Vector2f BraitenBoid::sensorOffset(10.f, 6.f);
const float BraitenBoid::sensorRadius = 2.f;
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
  /*
  Try with one nearby food source for now.

  std::vector<std::shared_ptr<FoodSource>> nearbyFoodSources =
      worldState.getNearbyFoodSources(
          pos, BraitenBoid::sensorRange + 2 * BraitenBoid::sensorOffset.norm());
  */

  std::shared_ptr<FoodSource> nearestFoodSource =
      worldState.getNearestFoodSource(pos);

  float leftSensorNeuron = 0;
  float rightSensorNeuron = 0;
  if (nearestFoodSource != nullptr) {
    // if (!nearbyFoodSources.empty()) {
    Rotation2Df sensorRot(atan2(dir.y(), dir.x()));
    Matrix2f sensorMat = sensorRot.toRotationMatrix();
    Vector2f leftSensorPos(BraitenBoid::sensorOffset.x(),
                           -BraitenBoid::sensorOffset.y());
    leftSensorPos = pos + sensorMat * leftSensorPos;
    Vector2f rightSensorPos(BraitenBoid::sensorOffset.x(),
                            BraitenBoid::sensorOffset.y());
    rightSensorPos = pos + sensorMat * rightSensorPos;

    float dist;

    dist = (nearestFoodSource->getPosition() - leftSensorPos).norm();
    leftSensorNeuron +=
        Util::linearInterp(dist, 0.f, BraitenBoid::sensorRange, 1.f, 0.f);
    dist = (nearestFoodSource->getPosition() - rightSensorPos).norm();
    rightSensorNeuron +=
        Util::linearInterp(dist, 0.f, BraitenBoid::sensorRange, 1.f, 0.f);

    /*
    for (auto &foodSource : nearbyFoodSources) {
      dist = (foodSource->position() - leftSensorPos).norm();
      if (dist < BraitenBoid::sensorRange) {
        // TODO: Try exponential activation of food sensor neurons.
        leftSensorNeuron +=
            Util::linearInterp(dist, 0.f, BraitenBoid::sensorRange, 0.5f, 0.f);
      }
      dist = (foodSource->position() - rightSensorPos).norm();
      if (dist < BraitenBoid::sensorRange) {
        rightSensorNeuron +=
            Util::linearInterp(dist, 0.f, BraitenBoid::sensorRange, 0.5f, 0.f);
      }
    }
    */

    leftSensorNeuron = std::clamp(leftSensorNeuron, 0.f, 1.f);
    rightSensorNeuron = std::clamp(rightSensorNeuron, 0.f, 1.f);
    //}
  }

  std::vector input({leftSensorNeuron, rightSensorNeuron});
  std::vector output = neuralNetwork.forward(input);
  float speedNeuron = output[0];
  float turnNeuron = output[1];

  float maxTurnDegrees = 1;
  float turnAmount = std::min(abs(turnNeuron), maxTurnDegrees * Util::pi / 180);
  float turn =
      turnAmount == 0 ? turnAmount : turnAmount * turnNeuron / abs(turnNeuron);
  Rotation2Df rotation(turn);
  dir = rotation.toRotationMatrix() * dir;

  speed = std::clamp(Util::linearInterp(speedNeuron, -1.f, 1.f, 0.f, maxSpeed),
                     0.f, maxSpeed);
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
