#include <SFML/Graphics.hpp>
#include "food_source_renderer.hpp"

FoodSourceRenderer::FoodSourceRenderer(FoodSource* fs, sf::Shape* s) {
	foodSource = fs;
	shape = s;
}