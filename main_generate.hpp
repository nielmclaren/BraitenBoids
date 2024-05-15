#pragma once

#include <fstream>
#include <iostream>

class MainGenerate {

  // TODO: Rename to indicate that it's an (open) unit ball.
  float randf() {
    return 2.f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) -
           1.f;
  }

public:
  MainGenerate(int argc, char *argv[]) {
    // Seed the random number generator.
    srand(static_cast<unsigned>(time(0)));

    std::cout << "generate command" << std::endl;

    int numBoids = 10;
    int numWeights = 6;

    std::string filename = "output/boids.csv";
    std::ofstream file(filename);

    file << "id, generation, numFoodsEaten, w0, w1, w2, w3, w4, w5"
         << std::endl;

    for (int i = 0; i < numBoids; i++) {
      // boid ID, generation index, number of food sources consumed
      file << i << ", " << 0 << ", " << 0 << ", ";

      // neural network weights
      for (int j = 0; j < numWeights; j++) {
        file << randf();
        if (j < numWeights - 1) {
          file << ", ";
        }
      }

      file << std::endl;
    }
  }
};
