#pragma once

#include <Eigen/Dense>
#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <iostream>
#include <vector>

using Eigen::MatrixXf;
using Eigen::Vector2f;
using Eigen::Vector3f;

class NeuralNetwork {
  friend class cereal::access;

  std::vector<float> weights;

  Vector3f inputVector;
  MatrixXf weightsMatrix;
  Vector2f outputVector;

  template <class Archive> void serialize(Archive &archive) {
    archive(CEREAL_NVP(weights));
  }

public:
  NeuralNetwork(unsigned int numInputs, unsigned int numOutputs,
                std::vector<float> weights);

  std::vector<float> getWeights() const;
  float &input(unsigned int n);
  float &output(unsigned int n);
  void forward();
  void reset();
};
