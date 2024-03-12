#pragma once

#include <SFML/Graphics.hpp>

class Simulation {
	// Pixels per second.
	float speed = 300.f;

public:
	sf::Vector2f position;

	void step(float timeDelta);
};