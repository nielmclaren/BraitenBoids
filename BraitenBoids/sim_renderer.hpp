#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "ifood_source_listener.hpp"

class SimRenderer : public IFoodSourceListener {
	Simulation *simulation;
	sf::RenderWindow *window;

	sf::CircleShape *shape;
	std::vector<sf::CircleShape> foodSourceShapes;

public:

	SimRenderer(Simulation& sim, sf::RenderWindow& win);
	~SimRenderer();
	void draw();
	void foodSourceCreated(FoodSource foodSource);
	void foodSourceDestroyed(FoodSource foodSource);
};