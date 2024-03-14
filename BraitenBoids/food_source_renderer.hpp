#pragma once

#include <SFML/Graphics.hpp>
#include "food_source.hpp"

class FoodSourceRenderer {
public:
	FoodSource *foodSource;
	sf::Shape *shape;

	FoodSourceRenderer(FoodSource* foodSource, sf::Shape* shape);
};