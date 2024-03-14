#include <iostream>
#include <SFML/Graphics.hpp>
#include "ifood_source_listener.hpp"
#include "simulation.hpp"

Simulation::Simulation(int w, int h) {
    size.x = w;
    size.y = h;

    position.x = size.x / 2;
    position.y = size.y / 2;
}

Simulation::~Simulation() {
    for (FoodSource* foodSource : foodSources) {
        delete foodSource;
    }
    foodSources.clear();

    foodSourceListeners.clear();
}

void Simulation::init() {
    initFoodSources();
}

void Simulation::initFoodSources() {
    std::cout << "Size: " << size.x << ", " << size.y << std::endl;
    int numFoodSources = 30;
    for (int i = 0; i < numFoodSources; i++) {
        FoodSource* foodSource = new FoodSource(sf::Vector2f(randf() * size.x, randf() * size.y));
        foodSources.push_back(foodSource);
        foodSourceCreated(foodSource);
    }
}

void Simulation::step(float timeDelta) {
    sf::Vector2f direction(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction.y += 1;
    }

    if (direction.x != 0 || direction.y != 0) {
        float len = sqrt(direction.x * direction.x + direction.y * direction.y);
        position.x += direction.x / len * speed * timeDelta;
        position.y += direction.y / len * speed * timeDelta;
    }
}

void Simulation::registerFoodSourceListener(IFoodSourceListener* listener) {
    foodSourceListeners.push_back(listener);
}

void Simulation::foodSourceCreated(FoodSource* foodSource) {
    for (std::vector<IFoodSourceListener*>::iterator it = begin(foodSourceListeners); it != end(foodSourceListeners); ++it) {
        (*it)->foodSourceCreated(foodSource);
    }
}

void Simulation::foodSourceDeleted(FoodSource* foodSource) {
    for (std::vector<IFoodSourceListener*>::iterator it = begin(foodSourceListeners); it != end(foodSourceListeners); ++it) {
        (*it)->foodSourceDeleted(foodSource);
    }
}

float Simulation::randf() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}