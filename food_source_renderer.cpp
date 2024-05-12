#include "food_source_renderer.hpp"
#include <SFML/Graphics.hpp>

FoodSourceRenderer::FoodSourceRenderer(FoodSource &foodSource)
    : foodSource(foodSource), shape(foodSource.radius) {
  float radius = foodSource.radius;
  shape.setFillColor(sf::Color(0, 128, 0));
  shape.setOrigin(radius, radius);
  shape.setPosition(eigenToSfml(foodSource.position));
}

void FoodSourceRenderer::draw(sf::RenderWindow &window) { window.draw(shape); }

sf::Vector2f FoodSourceRenderer::eigenToSfml(Eigen::Vector2f v) {
  return sf::Vector2f(v.x(), v.y());
}