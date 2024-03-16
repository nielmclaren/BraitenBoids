
#include <iostream>
#include <SFML/Graphics.hpp>
#include "food_source_renderer.hpp"
#include "simulation.hpp"
#include "sim_renderer.hpp"

SimRenderer::SimRenderer(Simulation &sim, sf::RenderWindow &win) {
	simulation = &sim;
	window = &win;

    float radius = simulation->avatar.radius;
    shape = new sf::CircleShape(radius);
    shape->setFillColor(sf::Color::Green);
    shape->setOrigin(radius, radius);

    simulation->registerBoidListener(this);
    simulation->registerFoodSourceListener(this);
}

SimRenderer::~SimRenderer() {
    delete shape;

    for (FoodSourceRenderer* renderer : foodSourceRenderers) {
        delete renderer;
    }
    foodSourceRenderers.clear();
}

void SimRenderer::draw() {
    window->clear();

    shape->setPosition(eigenToSfml(simulation->avatar.position));

    for (std::vector<BoidRenderer*>::iterator it = begin(boidRenderers); it != end(boidRenderers); ++it) {
        // Worth assuming the position changed every frame.
        (*it)->shape->setPosition(eigenToSfml((*it)->boid->position));

        window->draw(*((*it)->shape));
    }

    for (std::vector<FoodSourceRenderer*>::iterator it = begin(foodSourceRenderers); it != end(foodSourceRenderers); ++it) {
        window->draw(*((*it)->shape));
    }

    window->draw(*shape);

    window->display();
}


void SimRenderer::boidCreated(Boid* boid) {
    float radius = boid->radius;
    sf::Shape* shape = new sf::CircleShape(radius);
    shape->setFillColor(sf::Color(0, 128, 0));
    shape->setOrigin(radius, radius);
    shape->setPosition(eigenToSfml(boid->position));

    BoidRenderer* renderer = new BoidRenderer(boid, shape);
    boidRenderers.push_back(renderer);
}

void SimRenderer::boidDeleted(Boid* boid) {
    for (std::vector<BoidRenderer*>::iterator it = begin(boidRenderers); it != end(boidRenderers); ++it) {
        if ((*it)->boid->getId() == boid->getId()) {
            boidRenderers.erase(it);
            break;
        }
    }
}


void SimRenderer::foodSourceCreated(FoodSource* foodSource) {
    float radius = foodSource->radius;
    sf::Shape* shape = new sf::CircleShape(radius);
    shape->setFillColor(sf::Color(0, 0, 128));
    shape->setOrigin(radius, radius);
    shape->setPosition(eigenToSfml(foodSource->position));

    FoodSourceRenderer* renderer = new FoodSourceRenderer(foodSource, shape);
    foodSourceRenderers.push_back(renderer);
}

void SimRenderer::foodSourceDeleted(FoodSource* foodSource) {
    for (std::vector<FoodSourceRenderer*>::iterator it = begin(foodSourceRenderers); it != end(foodSourceRenderers); ++it) {
        if ((*it)->foodSource->getId() == foodSource->getId()) {
            foodSourceRenderers.erase(it);
            break;
        }
    }
}

sf::Vector2f SimRenderer::eigenToSfml(Eigen::Vector2f v) {
    return sf::Vector2f(v.x(), v.y());
}