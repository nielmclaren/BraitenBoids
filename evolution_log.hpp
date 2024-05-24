#pragma once

#include "evolution.hpp"
#include "simulation.hpp"
#include <string>
#include <vector>

struct EvolutionLogEntry {
  unsigned int generationIndex;
  float score;
};

class EvolutionLog {
  std::vector<EvolutionLogEntry> entries;

public:
  void addEntry(unsigned int generationIndex, float score);
  void addEntry(Simulation &simulation, unsigned int generationIndex,
                unsigned int stepCount);
  void clear();
  void save(std::string filename);
};
