#pragma once

#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include "screenshot.hpp"
#include "simulation.hpp"
#include "sim_renderer.hpp"

using Eigen::Vector2f;

class MainVisualize {
    sf::Clock clockwork;

    sf::RenderWindow window;
    Simulation simulation;
    SimRenderer simRenderer;
    unsigned int stepCount;
    unsigned int generationIndex;

    Screenshot screenshot;

    Vector2f getPlayerInputDirection();
    void handleEvent(sf::RenderWindow& window);

    void randomBoids(Simulation& simulation);
    void load(Simulation& simulation);
    void save(Simulation& simulation);
    void reportGenerationFitness(Simulation& simulation);
    void selectAndMutate(Simulation& simulation);
    float fitnessFunction(Boid& boid);
    std::vector<float> mutateWeights(std::vector<float> input);
    void fastForward(Simulation& simulation);

    float randf();
    std::string formatWeight(float weight);

public:
    MainVisualize(int argc, char* argv[]);
};

