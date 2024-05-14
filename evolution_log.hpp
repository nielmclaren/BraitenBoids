#pragma once

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
  void clear();
  void save(std::string filename);
};
