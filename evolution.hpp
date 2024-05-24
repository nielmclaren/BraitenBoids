#pragma once

#include "boid.hpp"
#include "simulation.hpp"

class Evolution {
  static std::vector<float> mutateWeights(std::vector<float> input);

public:
  static void selectAndMutate(Simulation &simulation);
  static float fitnessFunction(Boid &boid);
};
