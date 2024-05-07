#pragma once

#include <fstream>
#include <iostream>
#include <string>

class MainEvaluate {

	float randf() {
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}

public:
	MainEvaluate(int argc, char* argv[]) {

		// Seed the random number generator.
		srand(static_cast <unsigned> (time(0)));

		std::cout << "evaluate command" << std::endl;
	}
};

