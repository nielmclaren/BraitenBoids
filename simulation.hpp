#pragma once

#include "avatar.hpp"
#include "boid.hpp"
#include "food_source.hpp"
#include "iboid_listener.hpp"
#include "ifood_source_listener.hpp"
#include <Eigen/Dense>
#include <vector>

using Eigen::Vector2f;

class Simulation {
  // Pixels per second.
  float speed = 200;
  Vector2f playerDirection;

  void stepAvatar(float timeDelta);
  void stepBoids(float timeDelta);
  void stepFoodSources(float timeDelta);
  void handleCollisions();

  void boidCreated(Boid &boid);
  void boidDeleted(Boid &boid);

  void foodSourceCreated(FoodSource &foodSource);
  void foodSourceDeleted(FoodSource &foodSource);

  float randf();

public:
  static const unsigned int numInitialFoodSources = 30;

  Vector2f size;

  std::vector<Boid *> boids;
  std::vector<IBoidListener *> boidListeners;

  std::vector<FoodSource *> foodSources;
  std::vector<IFoodSourceListener *> foodSourceListeners;

  Avatar *avatar;

  Simulation(float w, float h);
  ~Simulation();

  void init();

  void resetFoodSources();

  void setPlayerDirection(Vector2f direction);
  void step(float timeDelta);

  void addBoid(BoidProps boidProps);
  void clearBoids();
  void setBoids(std::vector<BoidProps> boidPropses);

  FoodSource *getNearestFoodSource(Vector2f &point);
  float distanceToNearestFoodSource(Vector2f &point);
  std::vector<FoodSource *> getNearbyFoodSources(Vector2f &point, float range);

  void registerBoidListener(IBoidListener *listener);
  void registerFoodSourceListener(IFoodSourceListener *listener);
};