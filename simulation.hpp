#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "ifood_source_listener.hpp"
#include "avatar.hpp"
#include "food_source.hpp"

class Simulation {
	// Pixels per second.
	float speed = 300;

	void initFoodSources();
	void stepFoodSources();

	void foodSourceCreated(FoodSource* foodSource);
	void foodSourceDeleted(FoodSource* foodSource);
	
	float randf();

public:
	sf::Vector2i size;

	std::vector<FoodSource*> foodSources;
	std::vector<IFoodSourceListener*> foodSourceListeners;

	Avatar avatar;

	Simulation(int w, int h);
	~Simulation();

	void init();
	void step(float timeDelta);
	void registerFoodSourceListener(IFoodSourceListener* listener);
};