#pragma once

#include <vector>
#include <Eigen/Dense>
#include "avatar.hpp"
#include "boid.hpp"
#include "iboid_listener.hpp"
#include "ifood_source_listener.hpp"
#include "food_source.hpp"

using Eigen::Vector2f;

class Simulation {
	// Pixels per second.
	float speed = 200;
	Vector2f playerDirection;

	void initBoids();
	void initFoodSources();

	void stepAvatar(float timeDelta);
	void stepBoids(float timeDelta);
	void stepFoodSources(float timeDelta);

	void boidCreated(Boid* boid);
	void boidDeleted(Boid* boid);

	void foodSourceCreated(FoodSource* foodSource);
	void foodSourceDeleted(FoodSource* foodSource);
	
	float randf();

public:
	Vector2f size;

	std::vector<Boid*> boids;
	std::vector<IBoidListener*> boidListeners;

	std::vector<FoodSource*> foodSources;
	std::vector<IFoodSourceListener*> foodSourceListeners;

	Avatar avatar;

	Simulation(int w, int h);
	~Simulation();

	void init();
	void setPlayerDirection(Vector2f direction);
	void step(float timeDelta);

	void registerBoidListener(IBoidListener* listener);
	void registerFoodSourceListener(IFoodSourceListener* listener);
};