#include <iostream>
#include <SFML/Graphics.hpp>
#include "simulation.hpp"

Simulation::Simulation(int w, int h) {
    size.x = w;
    size.y = h;

    position.x = size.x / 2;
    position.y = size.y / 2;

    initFoodSources();
}

void Simulation::initFoodSources() {
    int numFoodSources = 30;
    for (int i = 0; i < numFoodSources; i++) {
        FoodSource* foodSource = new FoodSource(sf::Vector2f(rand() * size.x, rand() * size.y));
        foodSources.push_back(foodSource);
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