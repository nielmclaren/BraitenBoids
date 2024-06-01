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

  unsigned int numGenerations = 30;
  std::cout << "Total generations: " << numGenerations << std::endl;
  for (unsigned int generationIndex = 0; generationIndex < numGenerations;
       ++generationIndex) {
    std::cout << "Running generation " << generationIndex << "...";
    simulation.resetFoodSources();

    unsigned int stepCount = fastForward(simulation);

    unsigned int numFoodSourcesRemaining = simulation.getNumFoodSources();

    unsigned int foodConsumed =
        Simulation::numInitialFoodSources - numFoodSourcesRemaining;
    float foodConsumedPerStep =
        stepCount <= 0
            ? 0
            : static_cast<float>(foodConsumed) / static_cast<float>(stepCount);

    sf::Time elapsed = generationStopwatch.getElapsedTime();
    std::cout << " complete (" << elapsed.asSeconds() << " seconds, "
              << stepCount << " steps, " << foodConsumed << " consumed, "
              << printf("%.2f", foodConsumedPerStep) << " food/step)"
              << std::endl;
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
  unsigned int numTerminationFoodSources =
      static_cast<int>(Simulation::numInitialFoodSources * 0.2);
  unsigned int stepCount = simulation.fastForward(
      0.016f, 5000, [&numTerminationFoodSources](Simulation &simulation) {
        return simulation.getNumFoodSources() < numTerminationFoodSources;
      });
  return stepCount;
}
