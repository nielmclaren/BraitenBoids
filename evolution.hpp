#pragma once

#include "agent_props.hpp"
#include "simulation.hpp"

class Evolution {
  struct AgentFitness {
    AgentProps agent;
    float fitness;
  };

  static std::vector<float> mutateWeights(std::vector<float> input);
  static std::vector<AgentFitness>
  getAgentFitnessScores(std::vector<AgentProps> propses);

public:
  static void selectAndMutate(Simulation &simulation);
  static float fitnessFunction(AgentProps props);
  static void reportGenerationFitness(Simulation &simulation);
};
