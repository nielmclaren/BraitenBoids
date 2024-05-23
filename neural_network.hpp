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
  NeuralNetwork();
  NeuralNetwork(std::vector<float> weights);

  std::vector<float> getWeights() const;
  std::vector<float> forward(std::vector<float> input) const;
};
