
#include "sim_renderer.hpp"
#include "boid_renderer.hpp"
#include "braiten_boid_renderer.hpp"
#include "food_source_renderer.hpp"
#include "simulation.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using Eigen::Rotation2Df;
using Eigen::Vector2f;

SimRenderer::SimRenderer(Private p, Simulation &sim, sf::RenderWindow &win)
    : simulation(sim), window(win) {
  avatarRenderer = new AvatarRenderer(simulation.avatar);
}

SimRenderer::~SimRenderer() {
  delete avatarRenderer;

  for (IBoidRenderer *renderer : boidRenderers) {
    delete renderer;
  }
  boidRenderers.clear();

  for (FoodSourceRenderer *renderer : foodSourceRenderers) {
    delete renderer;
  }
  foodSourceRenderers.clear();
}

void SimRenderer::draw() {
  window.clear(sf::Color::White);

  for (std::vector<IBoidRenderer *>::iterator it = begin(boidRenderers);
       it != end(boidRenderers); ++it) {
    (*it)->draw(simulation, window);
  }

  for (std::vector<FoodSourceRenderer *>::iterator it =
           begin(foodSourceRenderers);
       it != end(foodSourceRenderers); ++it) {
    (*it)->draw(window);
  }

  avatarRenderer->draw(simulation, window);

  window.display();
}

void SimRenderer::entityCreated(IEntity &entity) {
  EntityType entityType = entity.getEntityType();
  switch (entityType) {
  case EntityType::Boid:
    boidCreated(*(dynamic_cast<Boid *>(&entity)));
    break;
  case EntityType::BraitenBoid:
    braitenBoidCreated(*(dynamic_cast<BraitenBoid *>(&entity)));
    break;
  case EntityType::FoodSource:
    foodSourceCreated(*(dynamic_cast<FoodSource *>(&entity)));
    break;
  default:

    // TODO: Never throw a string literal. Convert to exception class.
    throw "Unexpected entity type.";
  }
}

void SimRenderer::boidCreated(Boid &boid) {
  BoidRenderer *renderer = new BoidRenderer(boid);
  boidRenderers.push_back(renderer);
}

void SimRenderer::braitenBoidCreated(BraitenBoid &bBoid) {
  BraitenBoidRenderer *renderer = new BraitenBoidRenderer(bBoid);
  boidRenderers.push_back(renderer);
}

void SimRenderer::entityDeleted(IEntity &entity) {
  EntityType entityType = entity.getEntityType();
  switch (entityType) {
  case EntityType::Boid:
    boidDeleted(*(dynamic_cast<Boid *>(&entity)));
    break;
  case EntityType::BraitenBoid:
    braitenBoidDeleted(*(dynamic_cast<BraitenBoid *>(&entity)));
    break;
  case EntityType::FoodSource:
    foodSourceDeleted(*(dynamic_cast<FoodSource *>(&entity)));
    break;
  default:
    // TODO: Never throw a string literal. Convert to exception class.
    throw "Unexpected entity type.";
  }
}

void SimRenderer::boidDeleted(Boid &boid) {
  for (std::vector<IBoidRenderer *>::iterator it = begin(boidRenderers);
       it != end(boidRenderers); ++it) {
    if ((*it)->getBoidId() == boid.getId()) {
      delete (*it);
      boidRenderers.erase(it);
      break;
    }
  }
}

void SimRenderer::braitenBoidDeleted(BraitenBoid &bBoid) {
  for (std::vector<IBoidRenderer *>::iterator it = begin(boidRenderers);
       it != end(boidRenderers); ++it) {
    if ((*it)->getBoidId() == bBoid.getId()) {
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