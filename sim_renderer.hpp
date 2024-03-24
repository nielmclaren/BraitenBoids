#pragma once

#include <vector>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include "avatar_renderer.hpp"
#include "boid_renderer.hpp"
#include "food_source_renderer.hpp"
#include "ifood_source_listener.hpp"

class SimRenderer : public IBoidListener, public IFoodSourceListener {
	const float pi = std::acos(-1.0f);

	Simulation *simulation;
	sf::RenderWindow *window;

	AvatarRenderer* avatarRenderer;
	std::vector<BoidRenderer*> boidRenderers;
	std::vector<FoodSourceRenderer*> foodSourceRenderers;

	void drawBoid(BoidRenderer &renderer);

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