#pragma once

#include <SFML/Graphics.hpp>

class Boid {
	static unsigned int nextId;
	unsigned int id;

public:
	sf::Vector2f position;
	float radius;

	Boid(sf::Vector2f pos);
	unsigned int getId();

	void step(float timeDelta);
};

