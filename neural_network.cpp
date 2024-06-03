#include "neural_network.hpp"
#include <Eigen/Dense>

NeuralNetwork::NeuralNetwork(unsigned int numInputs, unsigned int numOutputs,
                             std::vector<float> weights)
    : numInputs(numInputs), numOutputs(numOutputs), weights(weights),
      inputVector(numInputs + 1), weightsMatrix(numOutputs, numInputs + 1),
      outputVector(numOutputs) {
  assert(weights.size() == (numInputs + 1) * numOutputs);
  inputVector(0) = 1.f; // Bias
  for (unsigned int col = 0; col < numInputs + 1; ++col) {
    for (unsigned int row = 0; row < numOutputs; ++row) {
      weightsMatrix(row, col) = weights[row * (numInputs + 1) + col];
    }
  }
}

float &NeuralNetwork::input(unsigned int n) {
  // Add one because of the bias node.
  return inputVector(n + 1);
}

float &NeuralNetwork::output(unsigned int n) { return outputVector(n); }

std::vector<float> NeuralNetwork::getWeights() const { return weights; }

void NeuralNetwork::forward() {
  // assert(inputVector(0) >= -1 && inputVector(0) <= 1);
  // assert(inputVector(1) >= -1 && inputVector(1) <= 1);

  outputVector = weightsMatrix * inputVector / (numInputs + 1.f);

  // assert(outputVector(0) >= -1 && outputVector(0) <= 1);
  // assert(outputVector(1) >= -1 && outputVector(1) <= 1);
}

void NeuralNetwork::reset() {
  inputVector.setZero();
  outputVector.setZero();
}