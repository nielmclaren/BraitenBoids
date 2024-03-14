#pragma once

#include <SFML/Graphics.hpp>

class FoodSource {
	static unsigned int nextId;
	unsigned int id;

public:
	sf::Vector2f position;
	float radius;

	FoodSource(sf::Vector2f pos);
	unsigned int getId();
};