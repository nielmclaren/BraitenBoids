#pragma once

#include <iostream>
#include <vector>
#include <Eigen/Dense>

class NeuralNetwork {
    std::vector<float> weights;

public:
    NeuralNetwork();

    std::vector<float> forward(std::vector<float> input);
};

