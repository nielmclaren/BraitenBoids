#include "braiten_boid_renderer.hpp"
#include "food_source.hpp"
#include "util.hpp"

BraitenBoidRenderer::BraitenBoidRenderer(const BraitenBoid &boid)
    : boid(boid), bodyShape(BraitenBoid::radius), directionShape(),
      leftSensorShape(BraitenBoid::sensorRadius),
      rightSensorShape(BraitenBoid::sensorRadius) {
  float radius = BraitenBoid::radius;

  int n = static_cast<int>(boid.getId()) % BraitenBoidRenderer::colors.size();
  sf::Color lightColor(BraitenBoidRenderer::lightColors[n]);
  sf::Color normalColor(BraitenBoidRenderer::colors[n]);

  bodyShape.setFillColor(lightColor);
  bodyShape.setOutlineColor(normalColor);
  bodyShape.setOutlineThickness(2);
  bodyShape.setOrigin(radius, radius);

  directionShape.setFillColor(normalColor);
  directionShape.setPointCount(3);
  directionShape.setPoint(0, sf::Vector2f(radius * 0.6f, 0));
  directionShape.setPoint(1, sf::Vector2f(-radius * 0.4f, -radius * 0.4f));
  directionShape.setPoint(2, sf::Vector2f(-radius * 0.4f, +radius * 0.4f));

  float sensorRadius = BraitenBoid::sensorRadius;
  leftSensorShape.setPosition(BraitenBoid::sensorOffset.x(),
                              -BraitenBoid::sensorOffset.y());
  leftSensorShape.setFillColor(lightColor);
  leftSensorShape.setOutlineColor(normalColor);
  leftSensorShape.setOutlineThickness(1);
  leftSensorShape.setOrigin(sensorRadius, sensorRadius);

  rightSensorShape.setPosition(BraitenBoid::sensorOffset.x(),
                               BraitenBoid::sensorOffset.y());
  rightSensorShape.setFillColor(lightColor);
  rightSensorShape.setOutlineColor(normalColor);
  rightSensorShape.setOutlineThickness(1);
  rightSensorShape.setOrigin(sensorRadius, sensorRadius);
}

unsigned int BraitenBoidRenderer::getBoidId() const { return boid.getId(); }

void BraitenBoidRenderer::draw(IWorldState &worldState,
                               sf::RenderWindow &window) {
  Vector2f position = boid.getPosition();
  Vector2f direction = boid.getDirection();

  // Worth assuming the position changed every frame.
  transform.setPosition(eigenToSfml(position));
  transform.setRotation(atan2(direction.y(), direction.x()) * 180 / Util::pi);

  window.draw(bodyShape, transform.getTransform());
  window.draw(directionShape, transform.getTransform());
  window.draw(leftSensorShape, transform.getTransform());
  window.draw(rightSensorShape, transform.getTransform());
}

sf::Vector2f BraitenBoidRenderer::eigenToSfml(Eigen::Vector2f v) {
  return sf::Vector2f(v.x(), v.y());
}

const std::vector<unsigned int> BraitenBoidRenderer::colors{
    0x4698bcff, // blue
    0x6bbb5dff, // green
    0xe18731ff, // orange
    0xa86cc5ff, // purple
    0xdf5d99ff, // pink
    0x48aa9fff, // teal
};
const std::vector<unsigned int> BraitenBoidRenderer::lightColors{
    0x8bc8d3ff, // light blue
    0xbcde85ff, // light green
    0xf5ac91ff, // light orange
    0xb4ace3ff, // light purple
    0xdea1d1ff, // light pink
    0x96d1b4ff, // light teal
};