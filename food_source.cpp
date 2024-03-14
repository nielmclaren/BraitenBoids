#include <SFML/Graphics.hpp>
#include "food_source.hpp"

unsigned int FoodSource::nextId = 0;

FoodSource::FoodSource(sf::Vector2f pos) {
	id = FoodSource::nextId++;
	position = pos;
}

unsigned int FoodSource::getId() {
	return id;
}