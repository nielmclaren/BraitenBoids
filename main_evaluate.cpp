#include "main_evaluate.hpp"
#include "boid_marshaller.hpp"
#include <format>

MainEvaluate::MainEvaluate(int argc, char *argv[])
    : simulation(800, 800), buildNamer("output/", "build", "/"),
      runNamer(buildNamer.curr(), "run", "/") {
  std::cout << "evaluate command" << std::endl;

  numGenerations = parseNumGenerations(argc, argv, 30);
  numRuns = parseNumRuns(argc, argv, 1);

  // Seed the random number generator.
  srand(static_cast<unsigned>(time(0)));

  simRunner = SimRunner::create(simulation);
  simulation.registerEntityListener(simRunner);

  std::string buildPath = buildNamer.curr();
  std::filesystem::create_directories(buildPath);

  totalStopwatch.restart();

  for (unsigned int run = 0; run < numRuns; ++run) {
    performRun(run);
  }

  sf::Time elapsed = totalStopwatch.getElapsedTime();
  std::cout << "Total time: " << elapsed.asSeconds() << "s" << std::endl;
}

unsigned int MainEvaluate::parseNumGenerations(int argc, char *argv[],
                                               unsigned int defaultValue) {
  if (argc <= 1)
    return defaultValue;

  for (int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], "-g") == 0) {
      if (i + 1 >= argc) {
        std::cerr << "Missing parameter value for number of generations -g."
                  << std::endl;
        throw "Missing parameter value for number of generations -g.";
        return defaultValue;
      }
      return std::stoi(argv[i + 1]);
    }
  }
  return defaultValue;
}

unsigned int MainEvaluate::parseNumRuns(int argc, char *argv[],
                                        unsigned int defaultValue) {
  if (argc <= 1)
    return defaultValue;

  for (int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], "-r") == 0) {
      if (i + 1 >= argc) {
        std::cerr << "Missing parameter value for number of runs -r."
                  << std::endl;
        throw "Missing parameter value for number of runs -r.";
        return defaultValue;
      }
      return std::stoi(argv[i + 1]);
    }
  }
  return defaultValue;
}

void MainEvaluate::performRun(unsigned int run) {
  std::cout << "Starting run " << (run + 1) << " of " << numRuns << std::endl;

  simRunner->resetAgents();
  evolutionLog.clear();

  runStopwatch.restart();

  for (unsigned int generation = 0; generation < numGenerations; ++generation) {
    performGeneration(run, generation);
  }

  sf::Time elapsed = runStopwatch.getElapsedTime();
  std::cout << "\tRun time: " << elapsed.asSeconds() << "s" << std::endl;

  std::string runPath = runNamer.curr();
  std::filesystem::create_directories(runPath);
  simRunner->saveAgents(runPath + "boids.json");
  evolutionLog.save(runPath + "evolution_log.csv");

  runNamer.next();
}

void MainEvaluate::performGeneration(unsigned int run,
                                     unsigned int generation) {

  std::cout << "\tRunning generation " << (generation + 1) << " of "
            << numGenerations << "...";

  simRunner->resetFoodSources();

  generationStopwatch.restart();

  unsigned int stepCount = simRunner->fastForward();

  unsigned int numFoodSourcesRemaining = simulation.getNumFoodSources();
  unsigned int foodConsumed =
      SimRunner::numFoodSources - numFoodSourcesRemaining;
  float foodConsumedPerStep =
      stepCount <= 0
          ? 0
          : static_cast<float>(foodConsumed) / static_cast<float>(stepCount);

  sf::Time elapsed = generationStopwatch.getElapsedTime();
  std::cout << " complete (" << elapsed.asSeconds() << " seconds, " << stepCount
            << " steps, " << foodConsumed << " consumed, "
            << printf("%.2f", foodConsumedPerStep) << " food/step)"
            << std::endl;

  evolutionLog.addEntry(simulation, generation, stepCount);
  simRunner->selectAndMutate();
}