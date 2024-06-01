#include "boid_marshaller.hpp"
#include "agent_props.hpp"
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
  std::vector<AgentProps> props;
  archiveIn(props);
  simulation.setAgents(props);
}

void BoidMarshaller::save(Simulation &simulation, std::string filename) {
  std::cout << "Saving file " << filename << std::endl;

  std::ofstream file(filename);
  cereal::JSONOutputArchive archiveOut(file);
  std::vector<AgentProps> props = simulation.getAgents();
  archiveOut(props);
}
