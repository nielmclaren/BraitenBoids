#include "simulation.hpp"
#include "collision_detection.hpp"
#include "util.hpp"
#include <Eigen/Dense>
#include <iostream>

using Eigen::Vector2f;

const float Simulation::playerSpeed = 200.f;

Simulation::Simulation(float w, float h) : width(w), height(h), tileMap(w, h) {
  avatar.position.x() = width / 2.f;
  avatar.position.y() = height / 2.f;

  avatar.direction.x() = 0;
  avatar.direction.y() = -1;
}

Simulation::~Simulation() {
  agents.clear();
  foodSources.clear();
  entityListeners.clear();
}

Vector2f Simulation::getSize() const { return Vector2f(width, height); }
unsigned int Simulation::getWidth() const { return width; }
unsigned int Simulation::getHeight() const { return height; }

const TileMap &Simulation::getTileMap() const { return tileMap; }

void Simulation::addFoodSource(Vector2f point) {
  std::shared_ptr<FoodSource> foodSource(new FoodSource(point));
  foodSources.push_back(foodSource);
  entityCreated(*foodSource);
}

void Simulation::clearFoodSources() {
  for (auto &foodSource : foodSources) {
    entityDeleted(*foodSource);
  }
  foodSources.clear();
}

unsigned int Simulation::getNumFoodSources() const {
  return static_cast<unsigned int>(foodSources.size());
}

void Simulation::setPlayerDirection(Vector2f direction) {
  playerDirection = direction;

  if (direction.norm() > 0) {
    avatar.direction = direction;
  }
}

void Simulation::step(float timeDelta) {
  stepAvatar(timeDelta);
  stepAgents(timeDelta);
  stepFoodSources(timeDelta);
  handleCollisions();
}

void Simulation::stepAvatar(float timeDelta) {
  avatar.position.x() += playerDirection.x() * playerSpeed * timeDelta;
  avatar.position.y() += playerDirection.y() * playerSpeed * timeDelta;

  while (avatar.position.x() < 0) {
    avatar.position.x() += width;
  }
  while (avatar.position.x() > width) {
    avatar.position.x() -= width;
  }
  while (avatar.position.y() < 0) {
    avatar.position.y() += height;
  }
  while (avatar.position.y() > height) {
    avatar.position.y() -= height;
  }
}

void Simulation::stepAgents(float timeDelta) {
  for (std::vector<std::shared_ptr<IAgent>>::iterator it = begin(agents);
       it != end(agents);) {
    std::shared_ptr<IAgent> agent = *it;
    if (agent->isDead()) {
      entityDeleted(*agent);
      it = agents.erase(it);
    } else {
      agent->step(*this, timeDelta);

      while (agent->position().x() < 0) {
        agent->position().x() += width;
      }
      while (agent->position().x() > width) {
        agent->position().x() -= width;
      }
      while (agent->position().y() < 0) {
        agent->position().y() += height;
      }
      while (agent->position().y() > height) {
        agent->position().y() -= height;
      }
      it++;
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

void Simulation::addAgent(AgentProps props) {
  Vector2f center(width / 2, height / 2);
  float jitter = 20;
  std::shared_ptr<IAgent> boid(
      new Boid(props, center + Vector2f(Util::randf(-jitter, jitter),
                                        Util::randf(-jitter, jitter))));
  agents.push_back(boid);
  entityCreated(*boid);
}

void Simulation::clearAgents() {
  for (auto &agent : agents) {
    entityDeleted(*agent);
  }
  agents.clear();
}

std::vector<AgentProps> Simulation::getAgents() const {
  std::vector<AgentProps> result;
  for (auto &agent : agents) {
    result.push_back(agent->toAgentProps());
  }
  return result;
}

unsigned int Simulation::getNumAgents() const { return agents.size(); }

void Simulation::setAgents(std::vector<AgentProps> propses) {
  clearAgents();
  for (auto &props : propses) {
    addAgent(props);
  }
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
