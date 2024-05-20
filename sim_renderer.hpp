#pragma once

#include "avatar_renderer.hpp"
#include "boid_renderer.hpp"
#include "food_source_renderer.hpp"
#include "iboid_listener.hpp"
#include "ifood_source_listener.hpp"
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include <vector>

class SimRenderer : public IBoidListener, public IFoodSourceListener {
  Simulation *simulation;
  sf::RenderWindow *window;

  AvatarRenderer *avatarRenderer;
  std::vector<BoidRenderer *> boidRenderers;
  std::vector<FoodSourceRenderer *> foodSourceRenderers;

  sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
  SimRenderer(Simulation &sim, sf::RenderWindow &win);
  ~SimRenderer();
  void draw();

  void boidCreated(Boid &boid);
  void boidDeleted(Boid &boid);

  void foodSourceCreated(FoodSource &foodSource);
  void foodSourceDeleted(FoodSource &foodSource);
};