#include <SFML/Graphics.hpp>

float x;
float y;

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

void handleKeyboard() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        x -= 3;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        x += 3;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        y -= 3;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        y += 3;
    }
}


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "BraitenBoids");
    window.setKeyRepeatEnabled(false);

    sf::Vector2u size = window.getSize();
    x = size.x / 2;
    y = size.y / 2;

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(100.f, 100.f);

    while (window.isOpen()) {
        handleEvent(window);
        handleKeyboard();

        shape.setPosition(x, y);

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}