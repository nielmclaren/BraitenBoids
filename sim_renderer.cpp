
#include <iostream>
#include <SFML/Graphics.hpp>
#include "food_source_renderer.hpp"
#include "simulation.hpp"
#include "sim_renderer.hpp"

using Eigen::Rotation2Df;
using Eigen::Vector2f;

SimRenderer::SimRenderer(Simulation &sim, sf::RenderWindow &win) {
	simulation = &sim;
	window = &win;

    avatarRenderer = new AvatarRenderer(simulation->avatar);

    simulation->registerBoidListener(this);
    simulation->registerFoodSourceListener(this);
}

SimRenderer::~SimRenderer() {
    delete avatarRenderer;

    for (BoidRenderer* renderer : boidRenderers) {
        delete renderer;
    }
    boidRenderers.clear();

    for (FoodSourceRenderer* renderer : foodSourceRenderers) {
        delete renderer;
    }
    foodSourceRenderers.clear();
}

void SimRenderer::draw() {
    window->clear();

    for (std::vector<BoidRenderer*>::iterator it = begin(boidRenderers); it != end(boidRenderers); ++it) {
        (*it)->draw(*window);
    }

    for (std::vector<FoodSourceRenderer*>::iterator it = begin(foodSourceRenderers); it != end(foodSourceRenderers); ++it) {
        window->draw(*((*it)->shape));
    }

    avatarRenderer->draw(*window);

    window->display();
}

void SimRenderer::boidCreated(Boid* boid) {
    BoidRenderer* renderer = new BoidRenderer(boid);
    boidRenderers.push_back(renderer);
}

void SimRenderer::boidDeleted(Boid* boid) {
    for (std::vector<BoidRenderer*>::iterator it = begin(boidRenderers); it != end(boidRenderers); ++it) {
        if ((*it)->boid->getId() == boid->getId()) {
            delete (*it);
            boidRenderers.erase(it);
            break;
        }
    }
}

void SimRenderer::foodSourceCreated(FoodSource* foodSource) {
    float radius = foodSource->radius;
    sf::Shape* shape = new sf::CircleShape(radius);
    shape->setFillColor(sf::Color(0, 128, 0));
    shape->setOrigin(radius, radius);
    shape->setPosition(eigenToSfml(foodSource->position));

    FoodSourceRenderer* renderer = new FoodSourceRenderer(foodSource, shape);
    foodSourceRenderers.push_back(renderer);
}

void SimRenderer::foodSourceDeleted(FoodSource* foodSource) {
    for (std::vector<FoodSourceRenderer*>::iterator it = begin(foodSourceRenderers); it != end(foodSourceRenderers); ++it) {
        if ((*it)->foodSource->getId() == foodSource->getId()) {
            delete (*it);
            foodSourceRenderers.erase(it);
            break;
        }
    }
}

sf::Vector2f SimRenderer::eigenToSfml(Eigen::Vector2f v) {
    return sf::Vector2f(v.x(), v.y());
}