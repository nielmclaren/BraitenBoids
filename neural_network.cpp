#include "neural_network.hpp"

NeuralNetwork::NeuralNetwork(unsigned int numInputs, unsigned int numOutputs,
                             std::vector<float> weights)
    : input(numInputs), output(numOutputs), weights(weights) {}

std::vector<float> NeuralNetwork::getWeights() const { return weights; }

void NeuralNetwork::forward() {
  assert(input.size() == 2);
  assert(input[0] >= -1 && input[0] <= 1);
  assert(input[1] >= -1 && input[1] <= 1);

  float bias = 1.0f;
  output[0] =
      (bias * weights[0] + input[0] * weights[1] + input[1] * weights[2]) / 3.f;
  output[1] =
      (bias * weights[3] + input[0] * weights[4] + input[1] * weights[5]) / 3.f;

  assert(output[0] >= -1 && output[0] <= 1);
  assert(output[1] >= -1 && output[1] <= 1);
}

void NeuralNetwork::reset() {
  std::fill(input.begin(), input.end(), 0);
  std::fill(output.begin(), output.end(), 0);
}