#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include "file_namer.hpp"
#include "screenshot.hpp"
#include "simulation.hpp"
#include "sim_renderer.hpp"

using Eigen::Vector2f;

sf::Clock clockwork;

Screenshot screenshot;

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
                if (event.key.scancode == sf::Keyboard::Scan::R) {
                    screenshot.capture(window);
                }
                if (event.key.scancode == sf::Keyboard::Scan::G) {
                    screenshot.toggleRecording();
                }
                break;
            }

            default: {
            }
        }
    }
}

Vector2f getPlayerInputDirection() {
    Vector2f direction(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x() -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x() += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y() -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction.y() += 1;
    }

    direction.normalize();
    return direction;
}

int main() {
    // Seed the random number generator.
    srand(static_cast <unsigned> (time(0)));

    sf::RenderWindow window(sf::VideoMode(800, 800), "BraitenBoids");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);

    sf::Vector2u size = window.getSize();
    Simulation simulation(static_cast<float>(size.x), static_cast<float>(size.y));
    SimRenderer simRenderer(simulation, window);
    simulation.init();

    while (window.isOpen()) {
        sf::Time elapsed = clockwork.getElapsedTime();
        float elapsedSeconds = elapsed.asSeconds();
        clockwork.restart();

        handleEvent(window);
        simulation.setPlayerDirection(getPlayerInputDirection());
        simulation.step(elapsedSeconds);
        simRenderer.draw();

        screenshot.step(window);
    }

    return 0;
}