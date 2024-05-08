#include "main_visualize.hpp"

MainVisualize::MainVisualize(int argc, char* argv[]) :
    window(sf::VideoMode(800, 800), "BraitenBoids"),
    simulation(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)),
    simRenderer(simulation, window)
{
    std::cout << "visualize command" << std::endl;

    // Seed the random number generator.
    srand(static_cast <unsigned> (time(0)));

    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);

    load(simulation);
    simulation.init();

    while (window.isOpen()) {
        sf::Time elapsed = clockwork.getElapsedTime();
        float elapsedSeconds = elapsed.asSeconds();
        clockwork.restart();

        handleEvent(window);
        simulation.setPlayerDirection(getPlayerInputDirection());
        simulation.step(elapsedSeconds);
        simRenderer.draw();

        screenshot.frameChanged(window);
    }
}

Vector2f MainVisualize::getPlayerInputDirection() {
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

void MainVisualize::handleEvent(sf::RenderWindow& window) {
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
            if (event.key.scancode == sf::Keyboard::Scan::V) {
                save(simulation);
            }
            break;
        }

        default: {
        }
        }
    }
}

void MainVisualize::load(Simulation& sim) {
    std::string filename = "boids.csv";
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::string line, colname, value;

    // Load column names.
    if (file.good()) {
        std::getline(file, line);
        std::stringstream ss(line);
        while (std::getline(ss, colname, ',')) {
            std::cout << "Column name: " << colname << " ";
        }
        std::cout << std::endl;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        BoidProps props;

        std::getline(ss, value, ',');
        props.id = stoi(value);
        std::getline(ss, value, ',');
        props.generationIndex = stoi(value);
        std::getline(ss, value, ',');
        int numFoodsEaten = stoi(value); // discard
        while (std::getline(ss, value, ',')) {
            props.weights.push_back(stof(value));
        }

        std::cout << "ID: " << props.id << ", gen: " << props.generationIndex << ", weights: ";
        for (auto& w : props.weights) {
            std::cout << w << " ";
        }
        std::cout << std::endl;

        sim.addBoid(props);
    }

    file.close();
}

void MainVisualize::save(Simulation& sim) {
    std::string filename = "boids_out.csv";
    std::cout << "Saving file " << filename << std::endl;

    std::ofstream file(filename);

    file << "id, generation, numFoodsEaten, w0, w1, w2, w3, w4, w5" << std::endl;

    for (auto& boid : sim.boids) {
        file << boid->getId() << ", " << (boid->getGenerationIndex() + 1) << ", " << boid->getNumFoodsEaten() << ", ";
        int numWeights = boid->getWeights().size();
        for (int i = 0; i < numWeights; ++i) {
            file << boid->getWeights()[i];
            if (i < numWeights - 1) {
                file << ", ";
            }
        }
        file << std::endl;
    }

    file.close();
}
