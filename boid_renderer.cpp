#include "boid_renderer.hpp"

BoidRenderer::BoidRenderer(Boid* b, sf::Shape* s) {
	boid = b;
	shape = s;
}