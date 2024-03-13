#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class SimRenderer {
	Simulation *simulation;
	sf::RenderWindow *window;

	sf::CircleShape *shape;

public:

	SimRenderer(Simulation& sim, sf::RenderWindow& win);
	~SimRenderer();
	void draw();
};