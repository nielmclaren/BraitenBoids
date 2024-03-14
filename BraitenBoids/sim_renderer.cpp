
#include <iostream>
#include <SFML/Graphics.hpp>
#include "food_source_renderer.hpp"
#include "simulation.hpp"
#include "sim_renderer.hpp"

SimRenderer::SimRenderer(Simulation &sim, sf::RenderWindow &win) {
	simulation = &sim;
	window = &win;

    float radius = 20.f;
    shape = new sf::CircleShape(radius);
    shape->setFillColor(sf::Color::Green);
    shape->setOrigin(radius, radius);

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

    shape->setPosition(simulation->position);

    for (std::vector<FoodSourceRenderer*>::iterator it = begin(foodSourceRenderers); it != end(foodSourceRenderers); ++it) {
        window->draw(*((*it)->shape));
    }

    window->draw(*shape);

    window->display();
}


void SimRenderer::foodSourceCreated(FoodSource* foodSource) {
    float radius = 50.f;
    sf::Shape* shape = new sf::CircleShape(radius);
    shape->setFillColor(sf::Color(0, 0, 128));
    shape->setOrigin(radius, radius);
    shape->setPosition(foodSource->position);

    std::cout << "Food sourdce created: " << foodSource->position.x << ", " << foodSource->position.y << std::endl;

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