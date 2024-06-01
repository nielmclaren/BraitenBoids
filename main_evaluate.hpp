#pragma once

#include "evolution_log.hpp"
#include "sim_runner.hpp"
#include "simulation.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>


class MainEvaluate {
  sf::Clock totalStopwatch;
  sf::Clock generationStopwatch;

  Simulation simulation;
  std::shared_ptr<SimRunner> simRunner;
  EvolutionLog evolutionLog;

public:
  MainEvaluate(int argc, char *argv[]);
};