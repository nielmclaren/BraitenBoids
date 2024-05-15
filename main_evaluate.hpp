#pragma once

#include "evolution_log.hpp"
#include "simulation.hpp"
#include <fstream>
#include <iostream>
#include <string>

class MainEvaluate {

  Simulation simulation;

  unsigned int stepCount;
  unsigned int generationIndex;

  float randf() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  }

public:
  MainEvaluate(int argc, char *argv[]);
};