#pragma once

#include <vector>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include "boid_renderer.hpp"
#include "food_source_renderer.hpp"
#include "ifood_source_listener.hpp"

class SimRenderer : public IBoidListener, public IFoodSourceListener {
	Simulation *simulation;
	sf::RenderWindow *window;

	sf::CircleShape *shape;
	std::vector<BoidRenderer*> boidRenderers;
	std::vector<FoodSourceRenderer*> foodSourceRenderers;

	sf::Vector2f eigenToSfml(Eigen::Vector2f v);

public:

	SimRenderer(Simulation& sim, sf::RenderWindow& win);
	~SimRenderer();
	void draw();

	void boidCreated(Boid* boid);
	void boidDeleted(Boid* boid);

	void foodSourceCreated(FoodSource* foodSource);
	void foodSourceDeleted(FoodSource* foodSource);
};