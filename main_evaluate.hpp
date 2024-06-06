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

  unsigned int numGenerations;
  unsigned int numRuns;
  std::string description;

  FileNamer buildNamer;
  FileNamer runNamer;

  unsigned int parseNumGenerations(int argc, char *argv[],
                                   unsigned int defaultValue);
  unsigned int parseNumRuns(int argc, char *argv[], unsigned int defaultValue);
  std::string parseDescription(int argc, char *argv[],
                               std::string defaultValue);

  void performRun(unsigned int run);
  void performGeneration(unsigned int run, unsigned int generation);
  void saveManifest();

public:
  MainEvaluate(int argc, char *argv[]);
};