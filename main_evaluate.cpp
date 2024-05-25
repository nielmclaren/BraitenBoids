#include "main_evaluate.hpp"
#include "boid_marshaller.hpp"
#include "evolution.hpp"
#include "util.hpp"
#include <format>

MainEvaluate::MainEvaluate(int argc, char *argv[]) : simulation(800, 800) {
  std::cout << "evaluate command" << std::endl;

  // Seed the random number generator.
  srand(static_cast<unsigned>(time(0)));

  simulation.resetBoids();
  simulation.resetFoodSources();

  totalStopwatch.restart();
  generationStopwatch.restart();

  int numGenerations = 30;
  for (int generationIndex = 0; generationIndex < numGenerations;
       ++generationIndex) {
    std::cout << "Running generation " << generationIndex << "...";
    simulation.resetFoodSources();

    unsigned int stepCount = fastForward(simulation);

    sf::Time elapsed = generationStopwatch.getElapsedTime();
    std::cout << " complete (" << elapsed.asSeconds() << " seconds, "
              << stepCount << " steps)" << std::endl;
    generationStopwatch.restart();

    evolutionLog.addEntry(simulation, generationIndex, stepCount);
    Evolution::selectAndMutate(simulation);
  }

  sf::Time elapsed = totalStopwatch.getElapsedTime();
  std::cout << "Total time: " << elapsed.asSeconds() << "s" << std::endl;

  BoidMarshaller::save(simulation, "output/boids.json");
  evolutionLog.save("output/evolution_log.csv");
}

unsigned int MainEvaluate::fastForward(Simulation &simulation) {
  unsigned int stepCount =
      simulation.fastForward(0.016f, 10000, [](Simulation &simulation) {
        return simulation.getNumFoodSources() < 7;
      });
  return stepCount;
}
