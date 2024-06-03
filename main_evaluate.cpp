#include "main_evaluate.hpp"
#include "boid_marshaller.hpp"
#include <format>

MainEvaluate::MainEvaluate(int argc, char *argv[]) : simulation(800, 800) {
  std::cout << "evaluate command" << std::endl;

  // Seed the random number generator.
  srand(static_cast<unsigned>(time(0)));

  simRunner = SimRunner::create(simulation);
  simulation.registerEntityListener(simRunner);

  simRunner->resetAgents();
  simRunner->resetFoodSources();

  totalStopwatch.restart();
  generationStopwatch.restart();

  unsigned int numGenerations = 30;
  std::cout << "Total generations: " << numGenerations << std::endl;
  for (unsigned int generationIndex = 0; generationIndex < numGenerations;
       ++generationIndex) {
    std::cout << "Running generation " << generationIndex << "...";
    simRunner->resetFoodSources();

    unsigned int stepCount = simRunner->fastForward();

    unsigned int numFoodSourcesRemaining = simulation.getNumFoodSources();

    unsigned int foodConsumed =
        SimRunner::numFoodSources - numFoodSourcesRemaining;
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
    simRunner->selectAndMutate();
  }

  sf::Time elapsed = totalStopwatch.getElapsedTime();
  std::cout << "Total time: " << elapsed.asSeconds() << "s" << std::endl;

  BoidMarshaller::save(simulation, "output/boids.json");
  evolutionLog.save("output/evolution_log.csv");
}
