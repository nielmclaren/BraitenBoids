#pragma once

#include <SFML/Graphics.hpp>
#include "boid.hpp"

class BoidRenderer {
public:
	Boid* boid;
	sf::CircleShape* body;
	sf::RectangleShape* toNearestFoodSource;

	BoidRenderer(Boid* boid, sf::CircleShape* bodyShape, sf::RectangleShape* toNearestFoodSourceShape);
};