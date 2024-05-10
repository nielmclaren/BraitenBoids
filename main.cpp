#include "main_evaluate.hpp"
#include "main_generate.hpp"
#include "main_visualize.hpp"
#include <iostream>

void printUsage() {
  std::cout << "Usage:" << std::endl;
  std::cout << "\t./BraitenBoids.exe generate" << std::endl;
  std::cout << "\t./BraitenBoids.exe evaluate" << std::endl;
  std::cout << "\t./BraitenBoids.exe select" << std::endl;
  std::cout << "\t./BraitenBoids.exe visualize" << std::endl;
}

int main(int argc, char *argv[]) {
  std::cout << "Command-line arguments:" << std::endl;
  for (int i(1); i < argc; ++i) {
    std::cout << "\t" << i << " = " << argv[i] << "\n";
  }

  if (argc < 2) {
    std::cout << "Missing required argument." << std::endl;
    printUsage();
    return 1;
  }

  if (!strcmp(argv[1], "generate")) {
    MainGenerate m(argc, argv);
    return 0;
  }

  if (!strcmp(argv[1], "evaluate")) {
    MainEvaluate m(argc, argv);
    return 0;
  }

  if (!strcmp(argv[1], "select")) {
    std::cout << "select command not implemented." << std::endl;
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