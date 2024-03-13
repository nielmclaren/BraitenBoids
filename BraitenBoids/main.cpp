#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "simulation.hpp"
#include "sim_renderer.hpp"

sf::Clock clockwork;

void handleEvent(sf::RenderWindow &window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                window.close();
                break;
            }

            case sf::Event::KeyReleased: {
                if (event.key.scancode == sf::Keyboard::Scan::Escape) {
                    window.close();
                }
                break;
            }

            default: {
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "BraitenBoids");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);

    sf::Vector2u size = window.getSize();
    Simulation simulation(size.x, size.y);
    SimRenderer simRenderer(simulation, window);

    std::vector<sf::CircleShape> foodSourceShapes;

    while (window.isOpen()) {
        sf::Time elapsed = clockwork.getElapsedTime();
        clockwork.restart();

        handleEvent(window);
        simulation.step(elapsed.asSeconds());
        simRenderer.draw();
    }

    return 0;
}