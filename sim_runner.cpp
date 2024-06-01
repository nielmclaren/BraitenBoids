#include "sim_runner.hpp"
#include "util.hpp"

SimRunner::SimRunner(Private p, Simulation &sim) : simulation(sim) {}

SimRunner::~SimRunner() {}

void SimRunner::resetAgents() {
  simulation.clearAgents();

  unsigned int numAgents = 10;
  unsigned int numWeights = 6;

  for (unsigned int i = 0; i < numAgents; ++i) {
    AgentProps props(i, 0, 0);
    for (unsigned int w = 0; w < numWeights; ++w) {
      props.weights.push_back(Util::randf(-1.f, 1.f));
    }
    simulation.addAgent(props);
  }
}

void SimRunner::resetFoodSources() {
  simulation.clearFoodSources();

  Vector2f size = simulation.getSize();

  Vector2f center(size.x() / 2, size.y() / 2);
  float noFoodZoneRadius = 110;
  for (unsigned int i = 0; i < SimRunner::numInitialFoodSources; i++) {
    Vector2f point(Util::randf(size.x()), Util::randf(size.y()));
    while ((point - center).norm() < noFoodZoneRadius) {
      // TODO Set the point in one line. (Eigen docs offline right now.)
      point(0) = Util::randf(size.x());
      point(1) = Util::randf(size.y());
    }

    simulation.addFoodSource(point);
  }
}

void SimRunner::step(float elapsedSeconds) { simulation.step(elapsedSeconds); }

void SimRunner::reportGenerationFitness() {
  std::vector<AgentProps> boids = simulation.getAgents();
  std::vector<AgentFitness> scores = getAgentFitnessScores(boids);

  std::cout << "\tFitness scores, weights: " << std::endl;
  for (auto &score : scores) {
    std::cout << "\t\t" << score.fitness << "\t";
    for (auto &w : score.agent.weights) {
      if (w >= 0) {
        // Stay aligned with negative numbers (minus sign).
        std::cout << " ";
      }
      printf("%.2f", w);
      std::cout << " ";
    }
    std::cout << std::endl;
  }
}

void SimRunner::selectAndMutate() {
  int population = 10;
  int selectNum = 4;

  std::vector<AgentProps> agents = simulation.getAgents();
  std::vector<AgentFitness> scores = getAgentFitnessScores(agents);

  std::vector<AgentProps> selected;
  std::transform(scores.begin(), scores.begin() + selectNum,
                 std::back_inserter(selected),
                 [](const AgentFitness &score) { return score.agent; });

  int nextId = 0;
  std::vector<AgentProps> mutated;
  while (mutated.size() < population) {
    for (auto &selectedProps : selected) {
      AgentProps mutatedProps;
      mutatedProps.id = ++nextId;
      mutatedProps.generationIndex = selectedProps.generationIndex + 1;
      mutatedProps.weights = mutateWeights(selectedProps.weights);
      mutated.push_back(mutatedProps);

      if (mutated.size() >= population) {
        break;
      }
    }
  }

  simulation.setAgents(mutated);
}

float SimRunner::fitnessFunction(AgentProps props) {
  return static_cast<float>(props.numFoodsEaten);
}

std::vector<float> SimRunner::mutateWeights(std::vector<float> weights) {
  std::vector<float> results;
  for (auto &w : weights) {
    results.push_back(std::clamp(w + Util::randf(-0.1f, 0.1f), -1.f, 1.f));
  }
  return results;
}

std::vector<SimRunner::AgentFitness>
SimRunner::getAgentFitnessScores(std::vector<AgentProps> propses) {
  std::vector<AgentFitness> scores;
  std::transform(propses.begin(), propses.end(), std::back_inserter(scores),
                 [](const AgentProps &props) {
                   AgentFitness score(props, fitnessFunction(props));
                   return score;
                 });

  sort(scores.begin(), scores.end(), [](const auto &lhs, const auto &rhs) {
    return lhs.fitness > rhs.fitness;
  });

  return scores;
}

unsigned int SimRunner::fastForward() {
  unsigned int numTerminationFoodSources =
      static_cast<int>(SimRunner::numInitialFoodSources * 0.2);
  float timeDelta = 0.016f;
  unsigned int maxSteps = 5000;
  for (unsigned int i = 0; i < maxSteps; i++) {
    simulation.step(timeDelta);
    if (simulation.getNumFoodSources() <= numTerminationFoodSources) {
      return i;
    }
  }
  return maxSteps;
}

void SimRunner::entityCreated(IEntity &entity) {}
void SimRunner::entityDeleted(IEntity &entity) {}
