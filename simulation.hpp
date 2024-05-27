#pragma once

#include "avatar.hpp"
#include "boid.hpp"
#include "food_source.hpp"
#include "ientity_listener.hpp"
#include "iworld_state.hpp"
#include <Eigen/Dense>
#include <functional>
#include <vector>

using Eigen::Vector2f;

class Simulation : public IWorldState {
  // Pixels per second.
  static const float playerSpeed;

  Vector2f playerDirection;

  std::vector<std::shared_ptr<IAgent>> agents;
  std::vector<std::shared_ptr<FoodSource>> foodSources;
  std::vector<std::weak_ptr<IEntityListener>> entityListeners;

  void stepAvatar(float timeDelta);
  void stepBoids(float timeDelta);
  void stepFoodSources(float timeDelta);
  void handleCollisions();

  void entityCreated(IEntity &entity);
  void entityDeleted(IEntity &entity);

public:
  static const unsigned int numInitialFoodSources = 10;

  Vector2f size;

  Avatar avatar;

  Simulation(float w, float h);
  ~Simulation();

  void clearFoodSources();
  void resetFoodSources();

  void setPlayerDirection(Vector2f direction);
  void step(float timeDelta);

  unsigned int
  fastForward(float timeDelta, unsigned int numSteps,
              std::function<bool(Simulation &)> earlyTerminationCondition);

  void addBoid(AgentProps agentProps);
  void clearBoids();
  std::vector<AgentProps> getBoids() const;
  void setBoids(std::vector<AgentProps> agentPropses);
  void resetBoids();

  unsigned int getNumFoodSources() const;

  std::shared_ptr<FoodSource> getNearestFoodSource(Vector2f &point) const;
  float distanceToNearestFoodSource(Vector2f &point) const;
  std::vector<std::shared_ptr<FoodSource>>
  getNearbyFoodSources(Vector2f &point, float range) const;

  void registerEntityListener(std::shared_ptr<IEntityListener> listener);
};