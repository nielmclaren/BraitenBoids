#include "boid_marshaller.hpp"
#include <fstream>
#include <iostream>

void BoidMarshaller::load(Simulation &simulation, std::string filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  simulation.clearBoids();

  std::string line, colname, value;

  // Load column names.
  if (file.good()) {
    std::getline(file, line);
    std::stringstream ss(line);
    while (std::getline(ss, colname, ',')) {
      std::cout << "Column name: " << colname << " ";
    }
    std::cout << std::endl;
  }

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    BoidProps props;

    std::getline(ss, value, ',');
    props.id = stoi(value);
    std::getline(ss, value, ',');
    props.generationIndex = stoi(value);
    std::getline(ss, value, ',');
    int numFoodsEaten = stoi(value); // discard
    while (std::getline(ss, value, ',')) {
      props.weights.push_back(stof(value));
    }

    std::cout << "ID: " << props.id << ", gen: " << props.generationIndex
              << ", weights: ";
    for (auto &w : props.weights) {
      std::cout << w << " ";
    }
    std::cout << std::endl;

    simulation.addBoid(props);
  }

  file.close();
}

void BoidMarshaller::save(Simulation &simulation, std::string filename) {
  std::cout << "Saving file " << filename << std::endl;

  std::ofstream file(filename);

  file << "id, generation, numFoodsEaten, w0, w1, w2, w3, w4, w5" << std::endl;

  for (auto &boid : simulation.boids) {
    file << boid->getId() << ", " << (boid->getGenerationIndex() + 1) << ", "
         << boid->getNumFoodsEaten() << ", ";
    int numWeights = static_cast<int>(boid->getWeights().size());
    for (int i = 0; i < numWeights; ++i) {
      file << boid->getWeights()[i];
      if (i < numWeights - 1) {
        file << ", ";
      }
    }
    file << std::endl;
  }

  file.close();
}