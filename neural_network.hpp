#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <vector>

class NeuralNetwork {
  std::vector<float> weights;

public:
  NeuralNetwork(std::vector<float> weights);

  std::vector<float> getWeights();
  std::vector<float> forward(std::vector<float> input);
};
