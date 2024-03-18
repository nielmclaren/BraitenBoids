#include <iostream>
#include <Eigen/Dense>
#include "collision_detection.hpp"
#include "simulation.hpp"

using Eigen::Vector2f;

Simulation::Simulation(int w, int h) {
    size.x() = w;
    size.y() = h;

    avatar.position.x() = w / 2.;
    avatar.position.y() = h / 2.;
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
        Boid* boid = new Boid(Vector2f(randf() * size.x(), randf() * size.y()));
        boids.push_back(boid);
        boidCreated(boid);
    }
}

void Simulation::initFoodSources() {
    int numFoodSources = 30;
    for (int i = 0; i < numFoodSources; i++) {
        FoodSource* foodSource = new FoodSource(Vector2f(randf() * size.x(), randf() * size.y()));
        foodSources.push_back(foodSource);
        foodSourceCreated(foodSource);
    }
}

void Simulation::setPlayerDirection(Vector2f direction) {
    playerDirection = direction;
}

void Simulation::step(float timeDelta) {
    stepAvatar(timeDelta);
    stepBoids(timeDelta);
    stepFoodSources(timeDelta);
}

void Simulation::stepAvatar(float timeDelta) {
    avatar.position.x() += playerDirection.x() * speed * timeDelta;
    avatar.position.y() += playerDirection.y() * speed * timeDelta;

    while (avatar.position.x() < 0) {
        avatar.position.x() += size.x();
    }
    while (avatar.position.x() > size.x()) {
        avatar.position.x() -= size.x();
    }
    while (avatar.position.y() < 0) {
        avatar.position.y() += size.y();
    }
    while (avatar.position.y() > size.y()) {
        avatar.position.y() -= size.y();
    }
}

void Simulation::stepBoids(float timeDelta) {
    for (std::vector<Boid*>::iterator it = begin(boids); it != end(boids); ++it) {
        Boid* boid = *it;
        boid->step(timeDelta);

        while (boid->position.x() < 0) {
            boid->position.x() += size.x();
        }
        while (boid->position.x() > size.x()) {
            boid->position.x() -= size.x();
        }
        while (boid->position.y() < 0) {
            boid->position.y() += size.y();
        }
        while (boid->position.y() > size.y()) {
            boid->position.y() -= size.y();
        }
    }
}

void Simulation::stepFoodSources(float timeDelta) {
    for (std::vector<FoodSource*>::iterator it = begin(foodSources); it != end(foodSources); ) {
        if (CollisionDetection::detect(avatar, **it)) {
            FoodSource* doomedFoodSource = *it;
            it = foodSources.erase(it);
            foodSourceDeleted(doomedFoodSource);
        }
        else {
            it++;
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