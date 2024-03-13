#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class Simulation {
	// Pixels per second.
	float speed = 300.f;

	void initFoodSources();

public:
	sf::Vector2i size;

	std::vector<sf::Vector2f> foodSources;

	sf::Vector2f position;

	Simulation(int w, int h);
	void step(float timeDelta);
};