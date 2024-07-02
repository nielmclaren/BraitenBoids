#pragma once

#include "avatar_renderer.hpp"
#include "food_source_renderer.hpp"
#include "iboid_renderer.hpp"
#include "ientity_listener.hpp"
#include "simulation.hpp"
#include "tile_map_renderer.hpp"
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include <vector>


class SimRenderer : public IEntityListener,
                    std::enable_shared_from_this<SimRenderer> {
  struct Private {};

  Simulation &simulation;
  sf::RenderWindow &window;

  bool displayAvatar = false;

  AvatarRenderer *avatarRenderer;
  std::vector<IBoidRenderer *> boidRenderers;
  std::vector<FoodSourceRenderer *> foodSourceRenderers;
  TileMapRenderer tileMapRenderer;

  void boidCreated(Boid &boid);
  void boidDeleted(Boid &boid);
  void foodSourceCreated(FoodSource &foodSource);
  void foodSourceDeleted(FoodSource &foodSource);

  sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
  SimRenderer(Private p, Simulation &sim, sf::RenderWindow &win);
  ~SimRenderer();
  static std::shared_ptr<SimRenderer> create(Simulation &sim,
                                             sf::RenderWindow &win) {
    // Create a shared pointer during construction so that a weak pointer can be
    // used for entity listener registration.
    return std::make_shared<SimRenderer>(Private(), sim, win);
  }

  void draw();

  void setDisplayAvatar(bool v);

  void entityCreated(IEntity &entity);
  void entityDeleted(IEntity &entity);
};