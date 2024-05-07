#pragma once

#include <string>
#include <vector>

class BoidProps {
public:
	unsigned int id;
	int generationIndex;
	int numFoodsEaten;

	std::vector<float> weights;

	BoidProps();
};

