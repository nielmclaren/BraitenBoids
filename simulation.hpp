#pragma once

#include "avatar.hpp"
#include "boid.hpp"
#include "food_source.hpp"
#include "ientity_listener.hpp"
#include "iworld_state.hpp"
#include <Eigen/Dense>
#include <vector>

using Eigen::Vector2f;

class Simulation : public IWorldState {
  // Pixels per second.
  float speed = 200;
  Vector2f playerDirection;

  void stepAvatar(float timeDelta);
  void stepBoids(float timeDelta);
  void stepFoodSources(float timeDelta);
  void handleCollisions();

  void entityCreated(IEntity &entity);
  void entityDeleted(IEntity &entity);

public:
  static const unsigned int numInitialFoodSources = 30;

  Vector2f size;

  std::vector<std::shared_ptr<Boid>> boids;
  std::vector<std::shared_ptr<FoodSource>> foodSources;
  std::vector<IEntityListener *> entityListeners;

  Avatar avatar;

  Simulation(float w, float h);
  ~Simulation();

  void clearFoodSources();
  void resetFoodSources();

  void setPlayerDirection(Vector2f direction);
  void step(float timeDelta);

  void addBoid(BoidProps boidProps);
  void clearBoids();
  std::vector<BoidProps> getBoids();
  void setBoids(std::vector<BoidProps> boidPropses);
  void setInitialBoids();

  std::shared_ptr<FoodSource> getNearestFoodSource(Vector2f &point);
  float distanceToNearestFoodSource(Vector2f &point);
  std::vector<std::shared_ptr<FoodSource>> getNearbyFoodSources(Vector2f &point,
                                                                float range);

  void registerEntityListener(IEntityListener *listener);
};