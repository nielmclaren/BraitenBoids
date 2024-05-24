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

  unsigned int fastForward(Simulation &simulation);
};