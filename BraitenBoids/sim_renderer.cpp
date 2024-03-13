
#include <SFML/Graphics.hpp>
#include "simulation.hpp"
#include "sim_renderer.hpp"

SimRenderer::SimRenderer(Simulation &sim, sf::RenderWindow &win) {
	simulation = &sim;
	window = &win;

    float radius = 20.f;
    shape = new sf::CircleShape(radius);
    shape->setFillColor(sf::Color::Green);
    shape->setOrigin(radius, radius);
}

SimRenderer::~SimRenderer() {
    delete shape;
}

void SimRenderer::draw() {
    window->clear();

    shape->setPosition(simulation->position);

    window->draw(*shape);

    window->display();
}


void SimRenderer::foodSourceCreated(FoodSource foodSource) {

}

void SimRenderer::foodSourceDestroyed(FoodSource foodSource) {

}