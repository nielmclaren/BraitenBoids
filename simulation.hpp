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

  Vector2f size;

  Vector2f playerDirection;

  std::vector<std::shared_ptr<IAgent>> agents;
  std::vector<std::shared_ptr<FoodSource>> foodSources;
  std::vector<std::weak_ptr<IEntityListener>> entityListeners;

  void stepAvatar(float timeDelta);
  void stepAgents(float timeDelta);
  void stepFoodSources(float timeDelta);
  void handleCollisions();

  void entityCreated(IEntity &entity);
  void entityDeleted(IEntity &entity);

public:
  Avatar avatar;

  Simulation(float w, float h);
  ~Simulation();

  Vector2f getSize();

  void addFoodSource(Vector2f point);
  void clearFoodSources();
  unsigned int getNumFoodSources() const;

  void setPlayerDirection(Vector2f direction);
  void step(float timeDelta);

  void addAgent(AgentProps agentProps);
  void clearAgents();
  std::vector<AgentProps> getAgents() const;
  void setAgents(std::vector<AgentProps> agentPropses);

  std::shared_ptr<FoodSource> getNearestFoodSource(Vector2f &point) const;
  float distanceToNearestFoodSource(Vector2f &point) const;
  std::vector<std::shared_ptr<FoodSource>>
  getNearbyFoodSources(Vector2f &point, float range) const;

  void registerEntityListener(std::shared_ptr<IEntityListener> listener);
};