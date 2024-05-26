#include "simulation.hpp"
#include "collision_detection.hpp"
#include "util.hpp"
#include <Eigen/Dense>
#include <iostream>

using Eigen::Vector2f;

const float Simulation::playerSpeed = 200.f;

Simulation::Simulation(float w, float h) {
  size.x() = w;
  size.y() = h;

  avatar.position.x() = w / 2.f;
  avatar.position.y() = h / 2.f;

  avatar.direction.x() = 0;
  avatar.direction.y() = -1;
}

Simulation::~Simulation() {
  agents.clear();
  foodSources.clear();
  entityListeners.clear();
}

void Simulation::clearFoodSources() {
  for (auto &foodSource : foodSources) {
    entityDeleted(*foodSource);
  }
  foodSources.clear();
}

void Simulation::resetFoodSources() {
  clearFoodSources();

  Vector2f center(size.x() / 2, size.y() / 2);
  float noFoodZoneRadius = 110;
  for (unsigned int i = 0; i < Simulation::numInitialFoodSources; i++) {
    Vector2f point(Util::randf(size.x()), Util::randf(size.y()));
    while ((point - center).norm() < noFoodZoneRadius) {
      // TODO Set the point in one line. (Eigen docs offline right now.)
      point(0) = Util::randf(size.x());
      point(1) = Util::randf(size.y());
    }

    std::shared_ptr<FoodSource> foodSource(new FoodSource(point));
    foodSources.push_back(foodSource);
    entityCreated(*foodSource);
  }
}

void Simulation::setPlayerDirection(Vector2f direction) {
  playerDirection = direction;

  if (direction.norm() > 0) {
    avatar.direction = direction;
  }
}

void Simulation::step(float timeDelta) {
  stepAvatar(timeDelta);
  stepBoids(timeDelta);
  stepFoodSources(timeDelta);
  handleCollisions();
}

unsigned int Simulation::fastForward(
    float timeDelta, unsigned int numSteps,
    std::function<bool(Simulation &)> earlyTerminationCondition) {
  int stepCount = 0;
  if (earlyTerminationCondition(*this)) {
    return stepCount;
  }

  for (unsigned int i = 0; i < numSteps; i++) {
    step(0.016f);
    stepCount++;

    if (earlyTerminationCondition(*this)) {
      break;
    }
  }
  return stepCount;
}

void Simulation::stepAvatar(float timeDelta) {
  avatar.position.x() += playerDirection.x() * playerSpeed * timeDelta;
  avatar.position.y() += playerDirection.y() * playerSpeed * timeDelta;

  while (avatar.position.x() < 0) {
    avatar.position.x() += size.x();
  }
  while (avatar.position.x() > size.x()) {
    avatar.position.x() -= size.x();
  }
  while (avatar.position.y() < 0) {
    avatar.position.y() += size.y();
  }
  while (avatar.position.y() > size.y()) {
    avatar.position.y() -= size.y();
  }
}

void Simulation::stepBoids(float timeDelta) {
  for (std::vector<std::shared_ptr<IAgent>>::iterator it = begin(agents);
       it != end(agents); ++it) {
    std::shared_ptr<IAgent> agent = *it;
    agent->step(*this, timeDelta);

    while (agent->position().x() < 0) {
      agent->position().x() += size.x();
    }
    while (agent->position().x() > size.x()) {
      agent->position().x() -= size.x();
    }
    while (agent->position().y() < 0) {
      agent->position().y() += size.y();
    }
    while (agent->position().y() > size.y()) {
      agent->position().y() -= size.y();
    }
  }
}

void Simulation::stepFoodSources(float timeDelta) {}

void Simulation::handleCollisions() {
  for (std::vector<std::shared_ptr<FoodSource>>::iterator it =
           begin(foodSources);
       it != end(foodSources);) {
    if (CollisionDetection::detect(avatar, **it)) {
      std::shared_ptr<FoodSource> doomedFoodSource = *it;

      doomedFoodSource->handleCollision(avatar);
      avatar.handleCollision(*doomedFoodSource);

      entityDeleted(*doomedFoodSource);
      it = foodSources.erase(it);
    } else {
      it++;
    }
  }

  for (std::vector<std::shared_ptr<IAgent>>::iterator agentIter = begin(agents);
       agentIter != end(agents); ++agentIter) {
    std::shared_ptr<IAgent> agent = *agentIter;

    for (std::vector<std::shared_ptr<FoodSource>>::iterator foodIter =
             begin(foodSources);
         foodIter != end(foodSources);) {
      std::shared_ptr<FoodSource> foodSource = *foodIter;

      if (CollisionDetection::detect(*agent, *foodSource)) {
        foodSource->handleCollision(*agent);
        agent->handleCollision(*foodSource);

        entityDeleted(*foodSource);
        foodIter = foodSources.erase(foodIter);
      } else {
        foodIter++;
      }
    }
  }
}

void Simulation::addBoid(AgentProps props) {
  Vector2f center(size.x() / 2, size.y() / 2);
  float jitter = 20;
  std::shared_ptr<IAgent> boid(
      new Boid(props, center + Vector2f(Util::randf(-jitter, jitter),
                                        Util::randf(-jitter, jitter))));
  agents.push_back(boid);
  entityCreated(*boid);
}

void Simulation::clearBoids() {
  for (auto &agent : agents) {
    entityDeleted(*agent);
  }
  agents.clear();
}

std::vector<AgentProps> Simulation::getBoids() const {
  std::vector<AgentProps> result;
  for (auto &agent : agents) {
    result.push_back(agent->toAgentProps());
  }
  return result;
}

void Simulation::setBoids(std::vector<AgentProps> propses) {
  clearBoids();
  for (auto &props : propses) {
    addBoid(props);
  }
}

void Simulation::resetBoids() {
  clearBoids();

  unsigned int numAgents = 10;
  unsigned int numWeights = 6;

  for (unsigned int i = 0; i < numAgents; ++i) {
    AgentProps props(i, 0, 0);
    for (unsigned int w = 0; w < numWeights; ++w) {
      props.weights.push_back(Util::randf(-1.f, 1.f));
    }
    addBoid(props);
  }
}

unsigned int Simulation::getNumFoodSources() const {
  return static_cast<unsigned int>(foodSources.size());
}

std::shared_ptr<FoodSource>
Simulation::getNearestFoodSource(Vector2f &point) const {
  std::shared_ptr<FoodSource> nearest = nullptr;
  float nearestDist = std::numeric_limits<float>::max();
  for (std::vector<std::shared_ptr<FoodSource>>::const_iterator it =
           begin(foodSources);
       it != end(foodSources); ++it) {
    std::shared_ptr<FoodSource> curr = *it;
    float dist = (curr->position() - point).norm();
    if (dist < nearestDist) {
      nearest = curr;
      nearestDist = dist;
    }
  }
  return nearest;
}

float Simulation::distanceToNearestFoodSource(Vector2f &point) const {
  if (foodSources.size() <= 0) {
    return -1;
  }

  float nearestDist = std::numeric_limits<float>::max();
  for (std::vector<std::shared_ptr<FoodSource>>::const_iterator it =
           begin(foodSources);
       it != end(foodSources); ++it) {
    std::shared_ptr<FoodSource> curr = *it;
    float dist = (curr->position() - point).norm();
    if (dist < nearestDist) {
      nearestDist = dist;
    }
  }
  return nearestDist;
}

std::vector<std::shared_ptr<FoodSource>>
Simulation::getNearbyFoodSources(Vector2f &point, float range) const {
  std::vector<std::shared_ptr<FoodSource>> result;
  for (std::vector<std::shared_ptr<FoodSource>>::const_iterator it =
           begin(foodSources);
       it != end(foodSources); ++it) {
    std::shared_ptr<FoodSource> curr = *it;
    float dist = (curr->position() - point).norm();
    if (dist < range) {
      result.push_back(curr);
    }
  }
  return result;
}

void Simulation::registerEntityListener(
    std::shared_ptr<IEntityListener> listener) {
  entityListeners.push_back(listener);
}

void Simulation::entityCreated(IEntity &entity) {
  for (std::vector<std::weak_ptr<IEntityListener>>::iterator it =
           begin(entityListeners);
       it != end(entityListeners); ++it) {
    if (std::shared_ptr<IEntityListener> listener = it->lock()) {
      listener->entityCreated(entity);
    } else {
      it = entityListeners.erase(it);
    }
  }
}

void Simulation::entityDeleted(IEntity &entity) {
  for (std::vector<std::weak_ptr<IEntityListener>>::iterator it =
           begin(entityListeners);
       it != end(entityListeners); ++it) {
    if (std::shared_ptr<IEntityListener> listener = it->lock()) {
      listener->entityDeleted(entity);
    } else {
      it = entityListeners.erase(it);
    }
  }
}
