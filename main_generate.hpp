#pragma once

#include <fstream>
#include <iostream>

class MainGenerate {

	float randf() {
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}

public:
	int main(int argc, char* argv[]) {
		// Seed the random number generator.
		srand(static_cast <unsigned> (time(0)));

		std::cout << "generate command" << std::endl;

		int numBoids = 10;
		int numWeights = 6;

		std::string filename = "boids.csv";
		std::ofstream file(filename);
		file << "id, numFoodsEaten, ";
		for (int i = 0; i < numWeights; i++) {
			file << "w" << i << ", ";
		}
		file << std::endl;

		for (int i = 0; i < numBoids; i++) {
			file << i << ", " << 0 << ", ";
			for (int j = 0; j < numWeights; j++) {
				file << randf();
				if (j < numWeights - 1) {
					file << ", ";
				}
			}
			file << std::endl;
		}

        file.close();

		return 0;
	}
};

