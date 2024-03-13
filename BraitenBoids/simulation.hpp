#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "food_source.hpp"

class Simulation {
	// Pixels per second.
	float speed = 300.f;

	void initFoodSources();

public:
	sf::Vector2i size;

	std::vector<FoodSource*> foodSources;

	sf::Vector2f position;

	Simulation(int w, int h);
	void step(float timeDelta);
};