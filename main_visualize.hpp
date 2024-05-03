#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include "screenshot.hpp"
#include "simulation.hpp"
#include "sim_renderer.hpp"

using Eigen::Vector2f;

class MainVisualize {
    sf::Clock clockwork;
    Screenshot screenshot;

    Vector2f getPlayerInputDirection();
    void handleEvent(sf::RenderWindow& window);

public:
    int main(int argc, char* argv[]);
};

