#pragma once

#include <Eigen/Dense>
#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <iostream>
#include <vector>

class NeuralNetwork {
  friend class cereal::access;

  std::vector<float> weights;

  template <class Archive> void serialize(Archive &archive) {
    archive(CEREAL_NVP(weights));
  }

public:
  std::vector<float> input;
  std::vector<float> output;

  NeuralNetwork(unsigned int numInputs, unsigned int numOutputs,
                std::vector<float> weights);

  std::vector<float> getWeights() const;
  void forward();
  void reset();
};
