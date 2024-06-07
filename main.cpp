#include "main_evaluate.hpp"
#include "main_visualize.hpp"
#include <iostream>

void printUsage() {
  std::cout << "Usage:" << std::endl;
  std::cout << "\t./BraitenBoids.exe evaluate -g <num_generations> -r "
               "<num_runs> -d <description>"
            << std::endl;
  std::cout << "\t./BraitenBoids.exe visualize -i path/to/boids.json"
            << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Missing required argument." << std::endl;
    printUsage();
    return 1;
  }

  if (!strcmp(argv[1], "evaluate")) {
    MainEvaluate m(argc, argv);
    return 0;
  }

  if (!strcmp(argv[1], "visualize")) {
    MainVisualize m(argc, argv);
    return 0;
  }

  std::cout << "Unknown command: " << argv[1] << std::endl;
  printUsage();
  return 1;
}