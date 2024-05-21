#pragma once

#include "avatar.hpp"
#include "boid.hpp"
#include "food_source.hpp"
#include "iboid_listener.hpp"
#include "ifood_source_listener.hpp"
#include "iworld_state.hpp"
#include <Eigen/Dense>
#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <vector>

using Eigen::Vector2f;

class Simulation : public IWorldState {
  friend class cereal::access;

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

  template <class Archive> void save(Archive &archive) const {
    archive(CEREAL_NVP(boids));
  }

  template <class Archive> void load(Archive &archive) {
    clearBoids();
    clearFoodSources();
    archive(CEREAL_NVP(boids));
    resetFoodSources();
  }

public:
  static const unsigned int numInitialFoodSources = 30;

  Vector2f size;

  std::vector<std::shared_ptr<Boid>> boids;
  std::vector<IBoidListener *> boidListeners;

  std::vector<std::shared_ptr<FoodSource>> foodSources;
  std::vector<IFoodSourceListener *> foodSourceListeners;

  Avatar avatar;

  Simulation();
  ~Simulation();

  void init(float w, float h);

  void clearFoodSources();
  void resetFoodSources();

  void setPlayerDirection(Vector2f direction);
  void step(float timeDelta);

  void addBoid(BoidProps boidProps);
  void clearBoids();
  void setBoids(std::vector<BoidProps> boidPropses);
  void boidsCreated();

  std::shared_ptr<FoodSource> getNearestFoodSource(Vector2f &point);
  float distanceToNearestFoodSource(Vector2f &point);
  std::vector<std::shared_ptr<FoodSource>> getNearbyFoodSources(Vector2f &point,
                                                                float range);

  void registerBoidListener(IBoidListener *listener);
  void registerFoodSourceListener(IFoodSourceListener *listener);
};