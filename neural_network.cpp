#include "neural_network.hpp"
#include <Eigen/Dense>

NeuralNetwork::NeuralNetwork(unsigned int numInputs, unsigned int numOutputs,
                             std::vector<float> weights)
    : weights(weights), weightsMatrix(numOutputs, numInputs + 1) {
  inputVector(0) = 1.f; // Bias
  weightsMatrix << weights[0], weights[1], weights[2], weights[3], weights[4],
      weights[5];
}

float &NeuralNetwork::input(unsigned int n) {
  // Add one because of bias.
  return inputVector(n + 1);
}

float &NeuralNetwork::output(unsigned int n) { return outputVector(n); }

std::vector<float> NeuralNetwork::getWeights() const { return weights; }

void NeuralNetwork::forward() {
  // assert(inputVector(0) >= -1 && inputVector(0) <= 1);
  // assert(inputVector(1) >= -1 && inputVector(1) <= 1);

  outputVector = weightsMatrix * inputVector / 3.f;

  // assert(outputVector(0) >= -1 && outputVector(0) <= 1);
  // assert(outputVector(1) >= -1 && outputVector(1) <= 1);
}

void NeuralNetwork::reset() {
  inputVector.setZero();
  outputVector.setZero();
}