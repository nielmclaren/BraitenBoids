#include "food_source.hpp"
#include "simulation.hpp"
#include "boid_renderer.hpp"

BoidRenderer::BoidRenderer(Boid* b) {
	boid = b;

    float radius = boid->radius;
    bodyShape = new sf::CircleShape(radius);
    bodyShape->setFillColor(sf::Color(0, 128, 0));
    bodyShape->setOrigin(radius, radius);
    bodyShape->setPosition(eigenToSfml(boid->position));

    toNearestFoodSourceShape = new sf::RectangleShape(sf::Vector2f(15.f, 1.f));
    toNearestFoodSourceShape->setFillColor(sf::Color::Green);

}

BoidRenderer::~BoidRenderer() {
    delete bodyShape;
    delete toNearestFoodSourceShape;
}

void BoidRenderer::draw(sf::RenderWindow& window) {
    Vector2f position = boid->position;
    Vector2f direction = boid->direction;

    // Worth assuming the position changed every frame.
    bodyShape->setPosition(eigenToSfml(position));
    window.draw(*bodyShape);
    FoodSource* nearestFoodSource = boid->simulation->getNearestFoodSource(position);
    if (nearestFoodSource != nullptr) {
        Vector2f toFoodSource = nearestFoodSource->position - position;
        float dist = toFoodSource.norm();
        if (dist <= boid->senseRadius) {
            toNearestFoodSourceShape->setPosition(eigenToSfml(position));
            toNearestFoodSourceShape->setSize(sf::Vector2f(dist, toNearestFoodSourceShape->getSize().y));
            toNearestFoodSourceShape->setRotation(atan2(toFoodSource.y(), toFoodSource.x()) * 180 / pi);
            window.draw(*toNearestFoodSourceShape);
        }
    }
}

sf::Vector2f BoidRenderer::eigenToSfml(Eigen::Vector2f v) {
    return sf::Vector2f(v.x(), v.y());
}