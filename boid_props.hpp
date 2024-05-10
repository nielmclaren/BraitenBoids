#pragma once

#include <string>
#include <vector>

class BoidProps {
public:
  unsigned int id;
  unsigned int generationIndex;
  unsigned int numFoodsEaten;

  std::vector<float> weights;

  BoidProps();
};
