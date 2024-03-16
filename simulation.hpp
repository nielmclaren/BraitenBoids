#pragma once

#include <vector>
#include "avatar.hpp"
#include "boid.hpp"
#include "iboid_listener.hpp"
#include "ifood_source_listener.hpp"
#include "food_source.hpp"

class Simulation {
	// Pixels per second.
	float speed = 300;

	void initBoids();
	void stepBoids(float timeDelta);

	void initFoodSources();
	void stepFoodSources(float timeDelta);

	void boidCreated(Boid* boid);
	void boidDeleted(Boid* boid);

	void foodSourceCreated(FoodSource* foodSource);
	void foodSourceDeleted(FoodSource* foodSource);
	
	float randf();

public:
	sf::Vector2i size;

	std::vector<Boid*> boids;
	std::vector<IBoidListener*> boidListeners;

	std::vector<FoodSource*> foodSources;
	std::vector<IFoodSourceListener*> foodSourceListeners;

	Avatar avatar;

	Simulation(int w, int h);
	~Simulation();

	void init();
	void step(float timeDelta);
	void registerBoidListener(IBoidListener* listener);
	void registerFoodSourceListener(IFoodSourceListener* listener);
};