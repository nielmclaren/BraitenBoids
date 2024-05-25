#include "boid_renderer.hpp"
#include "food_source.hpp"
#include "util.hpp"

BoidRenderer::BoidRenderer(const Boid &boid)
    : boid(boid), bodyShape(Boid::radius), directionShape(),
      toNearestFoodSourceShape(sf::Vector2f(15.f, 1.f)) {
  float radius = Boid::radius;

  int n = static_cast<int>(boid.getId()) % BoidRenderer::colors.size();
  sf::Color lightColor(BoidRenderer::lightColors[n]);
  sf::Color normalColor(BoidRenderer::colors[n]);

  bodyShape.setFillColor(lightColor);
  bodyShape.setOutlineColor(normalColor);
  bodyShape.setOutlineThickness(2);
  bodyShape.setOrigin(radius, radius);

  directionShape.setFillColor(normalColor);
  directionShape.setPointCount(3);
  directionShape.setPoint(0, sf::Vector2f(radius * 0.6f, 0));
  directionShape.setPoint(1, sf::Vector2f(-radius * 0.4f, -radius * 0.4f));
  directionShape.setPoint(2, sf::Vector2f(-radius * 0.4f, +radius * 0.4f));

  toNearestFoodSourceShape.setFillColor(lightColor);
}

void BoidRenderer::draw(IWorldState &worldState, sf::RenderWindow &window) {
  Vector2f position = boid.getPosition();
  Vector2f direction = boid.getDirection();

  // Worth assuming the position changed every frame.
  transform.setPosition(eigenToSfml(position));
  transform.setRotation(atan2(direction.y(), direction.x()) * 180 / Util::pi);

  std::shared_ptr<FoodSource> nearestFoodSource =
      worldState.getNearestFoodSource(position);
  if (nearestFoodSource != nullptr) {
    Vector2f toFoodSource = nearestFoodSource->getPosition() - position;
    float dist = toFoodSource.norm();
    if (dist <= Boid::senseRadius) {
      toNearestFoodSourceShape.setPosition(eigenToSfml(position));
      toNearestFoodSourceShape.setSize(
          sf::Vector2f(dist, toNearestFoodSourceShape.getSize().y));
      toNearestFoodSourceShape.setRotation(
          atan2(toFoodSource.y(), toFoodSource.x()) * 180 / Util::pi);
      window.draw(toNearestFoodSourceShape);
    }
  }

  window.draw(bodyShape, transform.getTransform());
  window.draw(directionShape, transform.getTransform());
}

sf::Vector2f BoidRenderer::eigenToSfml(Eigen::Vector2f v) {
  return sf::Vector2f(v.x(), v.y());
}

const std::vector<unsigned int> BoidRenderer::colors{
    0x4698bcff, // blue
    0x6bbb5dff, // green
    0xe18731ff, // orange
    0xa86cc5ff, // purple
    0xdf5d99ff, // pink
    0x48aa9fff, // teal
};
const std::vector<unsigned int> BoidRenderer::lightColors{
    0x8bc8d3ff, // light blue
    0xbcde85ff, // light green
    0xf5ac91ff, // light orange
    0xb4ace3ff, // light purple
    0xdea1d1ff, // light pink
    0x96d1b4ff, // light teal
};