#include "boid_renderer.hpp"
#include "food_source.hpp"
#include "simulation.hpp"

BoidRenderer::BoidRenderer(const Boid &boid)
    : boid(boid), bodyShape(boid.radius), directionShape(),
      toNearestFoodSourceShape(sf::Vector2f(15.f, 1.f)) {
  float radius = boid.radius;

  bodyShape.setFillColor(sf::Color(0, 128, 0));
  bodyShape.setOrigin(radius, radius);

  directionShape.setFillColor(sf::Color(33, 33, 33));
  directionShape.setPointCount(3);
  directionShape.setPoint(0, sf::Vector2f(radius * 0.6f, 0));
  directionShape.setPoint(1, sf::Vector2f(-radius * 0.4f, -radius * 0.4f));
  directionShape.setPoint(2, sf::Vector2f(-radius * 0.4f, +radius * 0.4f));

  toNearestFoodSourceShape.setFillColor(sf::Color::Green);
}

void BoidRenderer::draw(sf::RenderWindow &window) {
  Vector2f position = boid.position;
  Vector2f direction = boid.direction;

  // Worth assuming the position changed every frame.
  transform.setPosition(eigenToSfml(position));
  transform.setRotation(atan2(direction.y(), direction.x()) * 180 / pi);

  FoodSource *nearestFoodSource =
      boid.simulation->getNearestFoodSource(position);
  if (nearestFoodSource != nullptr) {
    Vector2f toFoodSource = nearestFoodSource->position - position;
    float dist = toFoodSource.norm();
    if (dist <= boid.senseRadius) {
      toNearestFoodSourceShape.setPosition(eigenToSfml(position));
      toNearestFoodSourceShape.setSize(
          sf::Vector2f(dist, toNearestFoodSourceShape.getSize().y));
      toNearestFoodSourceShape.setRotation(
          atan2(toFoodSource.y(), toFoodSource.x()) * 180 / pi);
      window.draw(toNearestFoodSourceShape);
    }
  }

  window.draw(bodyShape, transform.getTransform());
  window.draw(directionShape, transform.getTransform());
}

sf::Vector2f BoidRenderer::eigenToSfml(Eigen::Vector2f v) {
  return sf::Vector2f(v.x(), v.y());
}