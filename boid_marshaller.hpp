#pragma once
#include "simulation.hpp"
#include <string>

class BoidMarshaller {

  static float randf();

public:
  static void load(Simulation &simulation, std::string filename);
  static void save(Simulation &simulation, std::string filename);
  static void loadRandomBoids(Simulation &simulation);
};
