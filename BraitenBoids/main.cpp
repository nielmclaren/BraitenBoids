#include <iostream>
#include <SFML/Graphics.hpp>
#include "simulation.hpp"

Simulation simulation;
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
    simulation.position.x = size.x / 2;
    simulation.position.y = size.y / 2;

    float radius = 20.f;
    sf::CircleShape shape(radius);
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(radius, radius);

    while (window.isOpen()) {
        sf::Time elapsed = clockwork.getElapsedTime();
        clockwork.restart();

        handleEvent(window);
        simulation.step(elapsed.asSeconds());

        shape.setPosition(simulation.position);

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}