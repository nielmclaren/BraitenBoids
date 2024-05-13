
#include "sim_renderer.hpp"
#include "food_source_renderer.hpp"
#include "simulation.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using Eigen::Rotation2Df;
using Eigen::Vector2f;

SimRenderer::SimRenderer(Simulation &sim, sf::RenderWindow &win) {
  simulation = &sim;
  window = &win;

  avatarRenderer = new AvatarRenderer(*(simulation->avatar));

  simulation->registerBoidListener(this);
  simulation->registerFoodSourceListener(this);
}

SimRenderer::~SimRenderer() {
  delete avatarRenderer;

  for (BoidRenderer *renderer : boidRenderers) {
    delete renderer;
  }
  boidRenderers.clear();

  for (FoodSourceRenderer *renderer : foodSourceRenderers) {
    delete renderer;
  }
  foodSourceRenderers.clear();
}

void SimRenderer::draw() {
  window->clear(sf::Color::White);

  for (std::vector<BoidRenderer *>::iterator it = begin(boidRenderers);
       it != end(boidRenderers); ++it) {
    (*it)->draw(*window);
  }

  for (std::vector<FoodSourceRenderer *>::iterator it =
           begin(foodSourceRenderers);
       it != end(foodSourceRenderers); ++it) {
    (*it)->draw(*window);
  }

  avatarRenderer->draw(*window);

  window->display();
}

void SimRenderer::boidCreated(Boid &boid) {
  BoidRenderer *renderer = new BoidRenderer(boid);
  boidRenderers.push_back(renderer);
}

void SimRenderer::boidDeleted(Boid &boid) {
  for (std::vector<BoidRenderer *>::iterator it = begin(boidRenderers);
       it != end(boidRenderers); ++it) {
    if ((*it)->boid.getId() == boid.getId()) {
      delete (*it);
      boidRenderers.erase(it);
      break;
    }
  }
}

void SimRenderer::foodSourceCreated(FoodSource &foodSource) {
  FoodSourceRenderer *renderer = new FoodSourceRenderer(foodSource);
  foodSourceRenderers.push_back(renderer);
}

void SimRenderer::foodSourceDeleted(FoodSource &foodSource) {
  for (std::vector<FoodSourceRenderer *>::iterator it =
           begin(foodSourceRenderers);
       it != end(foodSourceRenderers); ++it) {
    if ((*it)->foodSource.getId() == foodSource.getId()) {
      delete (*it);
      foodSourceRenderers.erase(it);
      break;
    }
  }
}

sf::Vector2f SimRenderer::eigenToSfml(Eigen::Vector2f v) {
  return sf::Vector2f(v.x(), v.y());
}