#include <iostream>
#include <SFML/Graphics.hpp>

sf::Clock clockwork;

// Pixels per second.
float speed = 300.f;
sf::Vector2f position;

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

void step(float timeDelta) {
    sf::Vector2f direction(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction.y += 1;
    }

    if (direction.x != 0 || direction.y != 0) {
        float len = sqrt(direction.x * direction.x + direction.y * direction.y);
        position.x += direction.x / len * speed * timeDelta;
        position.y += direction.y / len * speed * timeDelta;
    }
}


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "BraitenBoids");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);

    sf::Vector2u size = window.getSize();
    position.x = size.x / 2;
    position.y = size.y / 2;

    float radius = 20.f;
    sf::CircleShape shape(radius);
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(radius, radius);

    while (window.isOpen()) {
        sf::Time elapsed = clockwork.getElapsedTime();
        clockwork.restart();

        handleEvent(window);
        step(elapsed.asSeconds());

        shape.setPosition(position);

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}