#include "boid_marshaller.hpp"
#include "boid_props.hpp"
#include "util.hpp"
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>
#include <iostream>

void BoidMarshaller::load(Simulation &simulation, std::string filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  cereal::JSONInputArchive archiveIn(file);
  simulation.clearBoids();
  simulation.clearFoodSources();
  archiveIn(simulation);
  simulation.boidsCreated();
  simulation.resetFoodSources();

  // TODO: Get rid of hard-coded simulation dimensions here.
  simulation.init(800, 800);
}

void BoidMarshaller::save(Simulation &simulation, std::string filename) {
  std::cout << "Saving file " << filename << std::endl;

  std::ofstream file(filename);
  cereal::JSONOutputArchive archiveOut(file);
  archiveOut(simulation);
}

void BoidMarshaller::loadRandomBoids(Simulation &simulation) {
  int numBoids = 10;
  int numWeights = 6;

  std::vector<BoidProps> boidPropses;
  for (int i = 0; i < numBoids; ++i) {
    BoidProps props;
    props.id = i;
    props.generationIndex = 0;

    for (int w = 0; w < numWeights; ++w) {
      props.weights.push_back(Util::randf(-1.f, 1.f));
    }
    boidPropses.push_back(props);
  }

  simulation.setBoids(boidPropses);
}
