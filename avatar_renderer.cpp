#include "avatar_renderer.hpp"
#include "food_source.hpp"
#include "simulation.hpp"

AvatarRenderer::AvatarRenderer(const Avatar &avatar)
    : avatar(avatar), bodyShape(avatar.radius),
      directionShape(sf::Vector2f(20, 1.f)),
      newDirectionShape(sf::Vector2f(30.f, 2.f)),
      toNearestFoodSourceShape(sf::Vector2f(15.f, 1.f)) {
  float radius = avatar.radius;
  bodyShape.setFillColor(sf::Color::Green);
  bodyShape.setOrigin(radius, radius);

  directionShape.setFillColor(sf::Color::Green);
  newDirectionShape.setFillColor(sf::Color::Green);
  toNearestFoodSourceShape.setFillColor(sf::Color::Green);
}

void AvatarRenderer::draw(sf::RenderWindow &window) {
  Vector2f position = avatar.position;
  Vector2f direction = avatar.direction;
  directionShape.setPosition(eigenToSfml(position));
  directionShape.setRotation(atan2(direction.y(), direction.x()) * 180 / pi);
  window.draw(directionShape);

  FoodSource *nearestFoodSource =
      avatar.simulation->getNearestFoodSource(position);
  if (nearestFoodSource != nullptr) {
    Vector2f toFoodSource = nearestFoodSource->position - position;
    float dist = (toFoodSource).norm();
    if (dist <= avatar.senseRadius) {
      toNearestFoodSourceShape.setPosition(eigenToSfml(position));
      toNearestFoodSourceShape.setSize(
          sf::Vector2f(dist, toNearestFoodSourceShape.getSize().y));
      toNearestFoodSourceShape.setRotation(
          atan2(toFoodSource.y(), toFoodSource.x()) * 180 / pi);
      window.draw(toNearestFoodSourceShape);

      float angleBetween = atan2(direction.x() * toFoodSource.y() -
                                     direction.y() * toFoodSource.x(),
                                 toFoodSource.dot(direction));
      Rotation2Df rotation(angleBetween * 0.1f);
      Vector2f newDirection = rotation.toRotationMatrix() * direction;

      newDirectionShape.setPosition(eigenToSfml(position));
      newDirectionShape.setRotation(atan2(newDirection.y(), newDirection.x()) *
                                    180 / pi);
      window.draw(newDirectionShape);
    }
  }

  bodyShape.setPosition(eigenToSfml(position));
  window.draw(bodyShape);
}

sf::Vector2f AvatarRenderer::eigenToSfml(Eigen::Vector2f v) {
  return sf::Vector2f(v.x(), v.y());
}