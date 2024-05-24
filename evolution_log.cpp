#include "evolution_log.hpp"

#include <fstream>
#include <iostream>

void EvolutionLog::addEntry(unsigned int generationIndex, float score) {
  EvolutionLogEntry entry(generationIndex, score);
  entries.push_back(entry);
}

void EvolutionLog::addEntry(Simulation &simulation,
                            unsigned int generationIndex,
                            unsigned int stepCount) {
  int numFoodSourcesRemaining = static_cast<int>(simulation.foodSources.size());
  float foodConsumedPerStep =
      stepCount <= 0 ? 0
                     : static_cast<float>(Simulation::numInitialFoodSources -
                                          numFoodSourcesRemaining) /
                           static_cast<float>(stepCount);
  addEntry(generationIndex, foodConsumedPerStep);
}

void EvolutionLog::clear() { entries.clear(); }

void EvolutionLog::save(std::string filename) {
  std::cout << "Saving evolution log file " << filename << std::endl;

  std::ofstream file(filename);

  file << "generation, score" << std::endl;

  for (auto &entry : entries) {
    file << entry.generationIndex << ", " << entry.score << std::endl;
  }
}
