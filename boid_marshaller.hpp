#pragma once
#include "simulation.hpp"
#include <string>

class BoidMarshaller {

public:
  static void load(Simulation &simulation, std::string filename);
  static void save(Simulation &simulation, std::string filename);
  static void loadRandomBoids(Simulation &simulation);
};
