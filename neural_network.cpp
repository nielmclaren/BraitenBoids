#include "neural_network.hpp"

NeuralNetwork::NeuralNetwork() {}

NeuralNetwork::NeuralNetwork(std::vector<float> weights) : weights(weights) {}
//	weights({ -0.2, 1, 0, 0, 0, 1 }) {}

std::vector<float> NeuralNetwork::getWeights() { return weights; }

std::vector<float> NeuralNetwork::forward(std::vector<float> input) {
  assert(input.size() == 2);
  assert(input[0] >= -1 && input[0] <= 1);
  assert(input[1] >= -1 && input[1] <= 1);

  float bias = 1.0f;
  std::vector output(
      {(bias * weights[0] + input[0] * weights[1] + input[1] * weights[2]) /
           3.f,
       (bias * weights[3] + input[0] * weights[4] + input[1] * weights[5]) /
           3.f});

  assert(output[0] >= -1 && output[0] <= 1);
  assert(output[1] >= -1 && output[1] <= 1);
  return output;
}