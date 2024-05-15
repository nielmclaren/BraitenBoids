#include "main_evaluate.hpp"

MainEvaluate::MainEvaluate(int argc, char *argv[])
    : simulation(800, 800), stepCount(0), generationIndex(0) {
  // Seed the random number generator.
  srand(static_cast<unsigned>(time(0)));

  std::cout << "evaluate command" << std::endl;
}
