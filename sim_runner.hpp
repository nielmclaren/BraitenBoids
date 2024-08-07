#pragma once

#include "ientity.hpp"
#include "ientity_listener.hpp"
#include "simulation.hpp"

// Runs one generation through the simulation.
class SimRunner : public IEntityListener,
                  std::enable_shared_from_this<SimRunner> {
  struct Private {};
  struct AgentFitness {
    AgentProps agent;
    float fitness;
  };

  Simulation &simulation;

  std::vector<AgentProps> deletedAgents;

  static std::vector<float> mutateWeights(std::vector<float> input);
  static std::vector<AgentFitness>
  getAgentFitnessScores(std::vector<AgentProps> propses);
  static float fitnessFunction(AgentProps props);

public:
  static const unsigned int numFoodSources = 60;
  static const unsigned int numAgents = 20;
  static const unsigned int numAgentWeights = 8;
  static const unsigned int numSelectionAgents = 4;
  static const unsigned int numFastForwardSteps = 5000;

  SimRunner(Private p, Simulation &sim);
  ~SimRunner();
  static std::shared_ptr<SimRunner> create(Simulation &sim) {
    // Create a shared pointer during construction so that a weak pointer can be
    // used for entity listener registration.
    return std::make_shared<SimRunner>(Private(), sim);
  }

  void resetAgents();
  void resetFoodSources();
  void step(float elapsedSeconds);
  void selectAndMutate();
  unsigned int fastForward();
  void reportGenerationFitness();

  void loadAgents(std::string path);
  void saveAgents(std::string path);

  void entityCreated(IEntity &entity);
  void entityDeleted(IEntity &entity);
};
