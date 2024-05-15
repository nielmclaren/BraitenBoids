#pragma once

#include "evolution_log.hpp"
#include "simulation.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>

class MainEvaluate {
  sf::Clock totalStopwatch;
  sf::Clock generationStopwatch;

  Simulation simulation;
  EvolutionLog evolutionLog;

  // TODO: Rename to indicate that it's an (open) unit ball.
  float randf() {
    return 2.f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) -
           1.f;
  }

public:
  MainEvaluate(int argc, char *argv[]);

  void logGeneration(Simulation &simulation, unsigned int generationIndex,
                     unsigned int stepCount);
  void selectAndMutate(Simulation &simulation);
  float fitnessFunction(Boid &boid);
  std::vector<float> mutateWeights(std::vector<float> input);
  unsigned int fastForward(Simulation &simulation);
};