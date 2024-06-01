#include "evolution.hpp"
#include "util.hpp"

void Evolution::reportGenerationFitness(Simulation &simulation) {
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

void Evolution::selectAndMutate(Simulation &simulation) {
  int population = 10;
  int selectNum = 4;

  std::vector<AgentProps> boids = simulation.getAgents();
  std::vector<AgentFitness> scores = getAgentFitnessScores(boids);

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
  simulation.resetFoodSources();
}

float Evolution::fitnessFunction(AgentProps props) {
  return static_cast<float>(props.numFoodsEaten);
}

std::vector<float> Evolution::mutateWeights(std::vector<float> weights) {
  std::vector<float> results;
  for (auto &w : weights) {
    results.push_back(std::clamp(w + Util::randf(-0.1f, 0.1f), -1.f, 1.f));
  }
  return results;
}

std::vector<Evolution::AgentFitness>
Evolution::getAgentFitnessScores(std::vector<AgentProps> propses) {
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