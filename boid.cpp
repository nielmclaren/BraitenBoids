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
      neuralNetwork(2, 2, props.weights), speed(0), pos(pos), energy(1) {
  dir << 1, 0;
  Rotation2Df rotation(Util::randf(2.f * Util::pi));
  dir = rotation.toRotationMatrix() * dir;
  vel = dir * speed;
}

Boid::~Boid() {}

unsigned int Boid::getId() const { return id; }
EntityType Boid::getEntityType() const { return EntityType::Boid; }
AgentType Boid::getAgentType() const { return AgentType::Boid; }

float Boid::getSpeed() const { return speed; };
Vector2f &Boid::position() { return pos; };
Vector2f Boid::getPosition() const { return pos; };
Vector2f &Boid::velocity() { return vel; };
Vector2f Boid::getVelocity() const { return vel; };
Vector2f &Boid::direction() { return dir; };
Vector2f Boid::getDirection() const { return dir; };
float Boid::getEnergy() const { return energy; };
bool Boid::isDead() const { return energy <= 0; };

std::vector<float> Boid::getWeights() const {
  return neuralNetwork.getWeights();
}

float Boid::getBoundingRadius() const { return radius; }

unsigned int Boid::getGenerationIndex() const { return generationIndex; }

int Boid::getNumFoodsEaten() const { return numFoodsEaten; }

void Boid::step(IWorldState &worldState, float timeDelta) {
  neuralNetwork.reset();

  float &detectionNeuron = neuralNetwork.input[0];
  float &directionNeuron = neuralNetwork.input[1];

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

  neuralNetwork.forward();
  const float &speedNeuron = neuralNetwork.output[0];
  const float &turnNeuron = neuralNetwork.output[1];

  float maxTurnDegrees = 1;
  float turnAmount = std::min(abs(turnNeuron), maxTurnDegrees * Util::pi / 180);
  float turn =
      turnAmount == 0 ? turnAmount : turnAmount * turnNeuron / abs(turnNeuron);
  Rotation2Df rotation(turn);
  dir = rotation.toRotationMatrix() * dir;

  speed = std::clamp(
      Util::linearInterp(speedNeuron, -1.f, 1.f, 0.f, Boid::maxSpeed), 0.f,
      Boid::maxSpeed);
  vel = dir * speed;

  pos += vel;

  energy -= speed * 0.002f;
}

void Boid::handleCollision(const ICollidable &collidable) {
  numFoodsEaten++;
  energy += 1.f;
  // std::cout << "Boid handle collision; numFoodsEaten=" << numFoodsEaten
  //           << std::endl;
}

AgentProps Boid::toAgentProps() const {
  AgentProps props;
  props.id = getId();
  props.generationIndex = getGenerationIndex();
  props.numFoodsEaten = getNumFoodsEaten();
  props.weights = getWeights();
  props.isDead = isDead();
  return props;
}
