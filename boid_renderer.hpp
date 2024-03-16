#pragma once

#include <SFML/Graphics.hpp>
#include "boid.hpp"

class BoidRenderer {
public:
	Boid* boid;
	sf::Shape* shape;

	BoidRenderer(Boid* boid, sf::Shape* shape);
};