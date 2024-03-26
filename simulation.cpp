#include <iostream>
#include <limits>
#include <Eigen/Dense>
#include "collision_detection.hpp"
#include "simulation.hpp"

using Eigen::Vector2f;

Simulation::Simulation(float w, float h) {
    size.x() = w;
    size.y() = h;

    avatar = new Avatar(this);
    avatar->position.x() = w / 2.f;
    avatar->position.y() = h / 2.f;

    avatar->direction.x() = 0;
    avatar->direction.y() = -1;
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
        Boid* boid = new Boid(this, Vector2f(randf() * size.x(), randf() * size.y()));
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

    if (direction.norm() > 0) {
        avatar->direction = direction;
    }
}

void Simulation::step(float timeDelta) {
    stepAvatar(timeDelta);
    stepBoids(timeDelta);
    stepFoodSources(timeDelta);
    handleCollisions();
}

void Simulation::stepAvatar(float timeDelta) {
    avatar->position.x() += playerDirection.x() * speed * timeDelta;
    avatar->position.y() += playerDirection.y() * speed * timeDelta;

    while (avatar->position.x() < 0) {
        avatar->position.x() += size.x();
    }
    while (avatar->position.x() > size.x()) {
        avatar->position.x() -= size.x();
    }
    while (avatar->position.y() < 0) {
        avatar->position.y() += size.y();
    }
    while (avatar->position.y() > size.y()) {
        avatar->position.y() -= size.y();
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

void Simulation::stepFoodSources(float timeDelta) {}

void Simulation::handleCollisions() {
    for (std::vector<FoodSource*>::iterator it = begin(foodSources); it != end(foodSources); ) {
        if (CollisionDetection::detect(*avatar, **it)) {
            FoodSource* doomedFoodSource = *it;

            doomedFoodSource->handleCollision(avatar);
            avatar->handleCollision(doomedFoodSource);

            foodSourceDeleted(doomedFoodSource);
            delete doomedFoodSource;
            it = foodSources.erase(it);
        }
        else {
            it++;
        }
    }

    for (std::vector<Boid*>::iterator boidIter = begin(boids); boidIter != end(boids); ++boidIter) {
        Boid* boid = *boidIter;

        for (std::vector<FoodSource*>::iterator foodIter = begin(foodSources); foodIter != end(foodSources); ) {
            FoodSource* foodSource = *foodIter;

            if (CollisionDetection::detect(*boid, *foodSource)) {
                foodSource->handleCollision(boid);
                boid->handleCollision(foodSource);

                foodSourceDeleted(foodSource);
                delete foodSource;
                foodIter = foodSources.erase(foodIter);
            }
            else {
                foodIter++;
            }
        }
    }
}

FoodSource* Simulation::getNearestFoodSource(Vector2f& point) {
    FoodSource* nearest = nullptr;
    float nearestDist = std::numeric_limits<float>::max();
    for (std::vector<FoodSource*>::iterator it = begin(foodSources); it != end(foodSources); ++it) {
        FoodSource* curr = *it;
        float dist = (curr->position - point).norm();
        if (dist < nearestDist) {
            nearest = curr;
            nearestDist = dist;
        }
    }
    return nearest;
}

float Simulation::distanceToNearestFoodSource(Vector2f& point) {
    if (foodSources.size() <= 0) {
        return -1;
    }

    float nearestDist = std::numeric_limits<float>::max();
    for (std::vector<FoodSource*>::iterator it = begin(foodSources); it != end(foodSources); ++it) {
        FoodSource* curr = *it;
        float dist = (curr->position - point).norm();
        if (dist < nearestDist) {
            nearestDist = dist;
        }
    }
    return nearestDist;

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