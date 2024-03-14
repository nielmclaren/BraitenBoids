#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "ifood_source_listener.hpp"
#include "food_source.hpp"

class Simulation {
	// Pixels per second.
	float speed = 300.f;

	void initFoodSources();
	void foodSourceCreated(FoodSource* foodSource);
	void foodSourceDeleted(FoodSource* foodSource);
	
	float randf();

public:
	sf::Vector2i size;

	std::vector<FoodSource*> foodSources;
	std::vector<IFoodSourceListener*> foodSourceListeners;

	sf::Vector2f position;

	Simulation(int w, int h);
	~Simulation();

	void init();
	void step(float timeDelta);
	void registerFoodSourceListener(IFoodSourceListener* listener);
};