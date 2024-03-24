#pragma once

#include <SFML/Graphics.hpp>
#include "boid.hpp"

class BoidRenderer {
	const float pi = std::acos(-1.0f);

	sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:
	Boid* boid;
	sf::CircleShape* bodyShape;
	sf::RectangleShape* toNearestFoodSourceShape;

	BoidRenderer(Boid* boid);
	~BoidRenderer();

	void draw(sf::RenderWindow& window);
};