
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
        drawBoid(**it);
    }

    for (std::vector<FoodSourceRenderer*>::iterator it = begin(foodSourceRenderers); it != end(foodSourceRenderers); ++it) {
        window->draw(*((*it)->shape));
    }

    avatarRenderer->draw(*window);

    window->display();
}

void SimRenderer::drawBoid(BoidRenderer &renderer) {
    // Worth assuming the position changed every frame.
    renderer.body->setPosition(eigenToSfml(renderer.boid->position));
    window->draw(*(renderer.body));

    Vector2f direction = renderer.boid->direction;
    Vector2f position = renderer.boid->position;
    FoodSource* nearestFoodSource = simulation->getNearestFoodSource(position);
    if (nearestFoodSource != nullptr) {
        Vector2f toFoodSource = nearestFoodSource->position - position;
        float dist = toFoodSource.norm();
        if (dist <= renderer.boid->senseRadius) {
            renderer.toNearestFoodSource->setPosition(eigenToSfml(position));
            renderer.toNearestFoodSource->setSize(sf::Vector2f(dist, renderer.toNearestFoodSource->getSize().y));
            renderer.toNearestFoodSource->setRotation(atan2(toFoodSource.y(), toFoodSource.x()) * 180 / pi);
            window->draw(*renderer.toNearestFoodSource);
        }
    }
}

void SimRenderer::boidCreated(Boid* boid) {
    float radius = boid->radius;
    sf::CircleShape* body = new sf::CircleShape(radius);
    body->setFillColor(sf::Color(0, 128, 0));
    body->setOrigin(radius, radius);
    body->setPosition(eigenToSfml(boid->position));

    sf::RectangleShape *toNearestFoodSource = new sf::RectangleShape(sf::Vector2f(15.f, 1.f));
    toNearestFoodSource->setFillColor(sf::Color::Green);

    BoidRenderer* renderer = new BoidRenderer(boid, body, toNearestFoodSource);
    boidRenderers.push_back(renderer);
}

void SimRenderer::boidDeleted(Boid* boid) {
    for (std::vector<BoidRenderer*>::iterator it = begin(boidRenderers); it != end(boidRenderers); ++it) {
        if ((*it)->boid->getId() == boid->getId()) {
            delete (*it)->body;
            delete (*it)->toNearestFoodSource;
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