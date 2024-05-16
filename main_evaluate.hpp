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

public:
  MainEvaluate(int argc, char *argv[]);

  void logGeneration(Simulation &simulation, unsigned int generationIndex,
                     unsigned int stepCount);
  void selectAndMutate(Simulation &simulation);
  float fitnessFunction(Boid &boid);
  std::vector<float> mutateWeights(std::vector<float> input);
  unsigned int fastForward(Simulation &simulation);
};