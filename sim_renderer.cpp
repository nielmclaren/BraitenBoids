
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

    avatarDirectionShape = new sf::RectangleShape(sf::Vector2f(20, 1.f));
    avatarDirectionShape->setFillColor(sf::Color::Green);
    avatarNewDirectionShape = new sf::RectangleShape(sf::Vector2f(30.f, 2.f));
    avatarNewDirectionShape->setFillColor(sf::Color::Green);
    avatarToFoodSourceShape = new sf::RectangleShape(sf::Vector2f(15.f, 1.f));
    avatarToFoodSourceShape->setFillColor(sf::Color::Green);

    float radius = simulation->avatar.radius;
    avatarShape = new sf::CircleShape(radius);
    avatarShape->setFillColor(sf::Color::Green);
    avatarShape->setOrigin(radius, radius);

    simulation->registerBoidListener(this);
    simulation->registerFoodSourceListener(this);
}

SimRenderer::~SimRenderer() {
    delete avatarDirectionShape;
    delete avatarNewDirectionShape;
    delete avatarToFoodSourceShape;
    delete avatarShape;

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

    drawAvatar();

    window->display();
}

void SimRenderer::drawAvatar() {
    Vector2f direction = simulation->avatar.direction;
    avatarDirectionShape->setPosition(eigenToSfml(simulation->avatar.position));
    avatarDirectionShape->setRotation(atan2(direction.y(), direction.x()) * 180 / pi);
    window->draw(*avatarDirectionShape);

    FoodSource* nearestFoodSource = simulation->getNearestFoodSource(simulation->avatar.position);
    if (nearestFoodSource != nullptr) {
        Vector2f toFoodSource = nearestFoodSource->position - simulation->avatar.position;
        float dist = (toFoodSource).norm();
        if (dist <= simulation->avatar.senseRadius) {
            avatarToFoodSourceShape->setPosition(eigenToSfml(simulation->avatar.position));
            avatarToFoodSourceShape->setSize(sf::Vector2f(dist, avatarToFoodSourceShape->getSize().y));
            avatarToFoodSourceShape->setRotation(atan2(toFoodSource.y(), toFoodSource.x()) * 180 / pi);
            window->draw(*avatarToFoodSourceShape);

            float angleBetween = atan2(direction.x() * toFoodSource.y() - direction.y() * toFoodSource.x(), toFoodSource.dot(direction));
            Rotation2Df rotation(angleBetween * 0.1f);
            Vector2f newDirection = rotation.toRotationMatrix() * direction;

            avatarNewDirectionShape->setPosition(eigenToSfml(simulation->avatar.position));
            avatarNewDirectionShape->setRotation(atan2(newDirection.y(), newDirection.x()) * 180 / pi);
            window->draw(*avatarNewDirectionShape);
        }
    }

    avatarShape->setPosition(eigenToSfml(simulation->avatar.position));
    window->draw(*avatarShape);
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