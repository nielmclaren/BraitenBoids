#include "main_generate.hpp"
#include "util.hpp"
#include <fstream>
#include <iostream>

MainGenerate::MainGenerate(int argc, char *argv[]) {
  // Seed the random number generator.
  srand(static_cast<unsigned>(time(0)));

  std::cout << "generate command" << std::endl;

  int numBoids = 10;
  int numWeights = 6;

  std::string filename = "output/boids.csv";
  std::ofstream file(filename);

  file << "id, generation, numFoodsEaten, w0, w1, w2, w3, w4, w5" << std::endl;

  for (int i = 0; i < numBoids; i++) {
    // boid ID, generation index, number of food sources consumed
    file << i << ", " << 0 << ", " << 0 << ", ";

    // neural network weights
    for (int j = 0; j < numWeights; j++) {
      file << Util::randf(-1.f, 1.f);
      if (j < numWeights - 1) {
        file << ", ";
      }
    }

    file << std::endl;
  }
}
