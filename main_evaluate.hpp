#pragma once

#include "evolution_log.hpp"
#include "file_namer.hpp"
#include "sim_runner.hpp"
#include "simulation.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>

class MainEvaluate {
  sf::Clock totalStopwatch;
  sf::Clock runStopwatch;
  sf::Clock generationStopwatch;

  Simulation simulation;
  std::shared_ptr<SimRunner> simRunner;
  EvolutionLog evolutionLog;

  FileNamer buildNamer;
  FileNamer runNamer;

  unsigned int parseNumGenerations(int argc, char *argv[],
                                   unsigned int defaultValue);
  unsigned int parseNumRuns(int argc, char *argv[], unsigned int defaultValue);

public:
  MainEvaluate(int argc, char *argv[]);
};