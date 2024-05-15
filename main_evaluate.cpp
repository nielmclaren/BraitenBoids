#include "main_evaluate.hpp"
#include "boid_marshaller.hpp"
#include <format>

MainEvaluate::MainEvaluate(int argc, char *argv[]) : simulation(800, 800) {
  // Seed the random number generator.
  srand(static_cast<unsigned>(time(0)));

  std::cout << "evaluate command" << std::endl;

  BoidMarshaller::loadRandomBoids(simulation);

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

    logGeneration(simulation, generationIndex, stepCount);
    selectAndMutate(simulation);
  }

  sf::Time elapsed = totalStopwatch.getElapsedTime();
  std::cout << "Total time: " << elapsed.asSeconds() << "s" << std::endl;

  BoidMarshaller::save(simulation, "output/boids.csv");
  evolutionLog.save("output/evolution_log.csv");
}

// TODO: DRY this code (MainVisualize).
void MainEvaluate::logGeneration(Simulation &simulation,
                                 unsigned int generationIndex,
                                 unsigned int stepCount) {
  std::vector<Boid *> boids = simulation.boids;
  for (auto &boid : boids) {
    boid->fitnessScore = fitnessFunction(*boid);
  }
  sort(boids.begin(), boids.end(), [](const auto &lhs, const auto &rhs) {
    return lhs->fitnessScore > rhs->fitnessScore;
  });

  int numFoodSourcesRemaining = static_cast<int>(simulation.foodSources.size());
  float foodConsumedPerStep =
      stepCount <= 0 ? 0
                     : static_cast<float>(Simulation::numInitialFoodSources -
                                          numFoodSourcesRemaining) /
                           static_cast<float>(stepCount);
  evolutionLog.addEntry(generationIndex, foodConsumedPerStep);
}

// TODO: DRY this code (MainVisualize).
void MainEvaluate::selectAndMutate(Simulation &simulation) {
  int population = 10;
  int selectNum = 4;

  std::vector<Boid *> boids = simulation.boids;
  for (auto &boid : boids) {
    boid->fitnessScore = fitnessFunction(*boid);
  }

  sort(boids.begin(), boids.end(), [](const auto &lhs, const auto &rhs) {
    return lhs->fitnessScore > rhs->fitnessScore;
  });

  std::vector<Boid *> selected(boids.begin(), boids.begin() + selectNum);

  int nextId = 0;
  std::vector<BoidProps> mutated;
  while (mutated.size() < population) {
    for (auto &boid : selected) {
      BoidProps props;
      props.id = ++nextId;
      props.generationIndex = boid->getGenerationIndex() + 1;
      props.weights = mutateWeights(boid->getWeights());

      mutated.push_back(props);

      if (mutated.size() >= population) {
        break;
      }
    }
  }

  simulation.setBoids(mutated);
  simulation.resetFoodSources();
}

// TODO: DRY this code (MainVisualize).
float MainEvaluate::fitnessFunction(Boid &boid) {
  return static_cast<float>(boid.getNumFoodsEaten());
}

// TODO: DRY this code (MainVisualize).
std::vector<float> MainEvaluate::mutateWeights(std::vector<float> weights) {
  std::vector<float> results;
  for (auto &w : weights) {
    results.push_back(std::clamp(w + randf() * 0.1f, -1.f, 1.f));
  }
  std::cout << "\t"
            << "Mutation: ";
  for (int i = 0; i < weights.size(); ++i) {
    std::cout << (weights[i] >= 0 ? " " : "")
              << std::format("{:.1f}", weights[i]) << " ";
  }
  std::cout << " --> ";
  for (int i = 0; i < results.size(); ++i) {
    std::cout << (results[i] >= 0 ? " " : "")
              << std::format("{:.1f}", results[i]) << " ";
  }
  std::cout << std::endl;
  return results;
}

// TODO: DRY this code (MainVisualize).
unsigned int MainEvaluate::fastForward(Simulation &simulation) {
  int stepCount = 0;
  for (int i = 0; i < 10000; i++) {
    simulation.step(0.016f);
    stepCount++;

    if (simulation.foodSources.size() < 7) {
      break;
    }
  }
  return stepCount;
}
