#include "boid_renderer.hpp"

BoidRenderer::BoidRenderer(Boid* b, sf::CircleShape* bodyShape, sf::RectangleShape* toNearestFoodSourceShape) {
	boid = b;
	body = bodyShape;
	toNearestFoodSource = toNearestFoodSourceShape;
}
