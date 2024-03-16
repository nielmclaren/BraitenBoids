#include <iostream>
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include "collision_detection.hpp"
#include "simulation.hpp"

using Eigen::Vector2f;

Simulation::Simulation(int w, int h) {
    size.x = w;
    size.y = h;

    avatar.position.x() = size.x / 2.;
    avatar.position.y() = size.y / 2.;
}

Simulation::~Simulation() {
    for (Boid* boid : boids) {
        delete boid;
    }
    boids.clear();

    for (FoodSource* foodSource : foodSources) {
        delete foodSource;
    }
    foodSources.clear();

    foodSourceListeners.clear();
}

void Simulation::init() {
    initBoids();
    initFoodSources();
}

void Simulation::initBoids() {
    int numBoids = 10;
    for (int i = 0; i < numBoids; i++) {
        Boid* boid = new Boid(Vector2f(randf() * size.x, randf() * size.y));
        boids.push_back(boid);
        boidCreated(boid);
    }
}

void Simulation::initFoodSources() {
    int numFoodSources = 30;
    for (int i = 0; i < numFoodSources; i++) {
        FoodSource* foodSource = new FoodSource(Vector2f(randf() * size.x, randf() * size.y));
        foodSources.push_back(foodSource);
        foodSourceCreated(foodSource);
    }
}

void Simulation::step(float timeDelta) {
    Vector2f direction(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x() -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x() += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y() -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction.y() += 1;
    }

    if (direction.x() != 0 || direction.y() != 0) {
        float m = direction.size();
        avatar.position.x() += direction.x() / m * speed * timeDelta;
        avatar.position.y() += direction.y() / m * speed * timeDelta;
    }

    stepBoids(timeDelta);
    stepFoodSources(timeDelta);
}

void Simulation::stepBoids(float timeDelta) {
    for (std::vector<Boid*>::iterator it = begin(boids); it != end(boids); ++it) {
        (*it)->step(timeDelta);
    }
}

void Simulation::stepFoodSources(float timeDelta) {
    for (std::vector<FoodSource*>::iterator it = begin(foodSources); it != end(foodSources); ++it) {
        if (CollisionDetection::detect(avatar, **it)) {
            FoodSource* doomedFoodSource = *it;
            it = foodSources.erase(it);
            foodSourceDeleted(doomedFoodSource);
        }
    }
}


void Simulation::registerBoidListener(IBoidListener* listener) {
    boidListeners.push_back(listener);
}

void Simulation::boidCreated(Boid* boid) {
    for (std::vector<IBoidListener*>::iterator it = begin(boidListeners); it != end(boidListeners); ++it) {
        (*it)->boidCreated(boid);
    }
}

void Simulation::boidDeleted(Boid* boid) {
    for (std::vector<IBoidListener*>::iterator it = begin(boidListeners); it != end(boidListeners); ++it) {
        (*it)->boidDeleted(boid);
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