#include "evolution.hpp"
#include "util.hpp"

void Evolution::selectAndMutate(Simulation &simulation) {
  int population = 10;
  int selectNum = 4;

  std::vector<std::shared_ptr<Boid>> boids = simulation.boids;
  for (auto &boid : boids) {
    boid->fitnessScore = fitnessFunction(*boid);
  }

  sort(boids.begin(), boids.end(), [](const auto &lhs, const auto &rhs) {
    return lhs->fitnessScore > rhs->fitnessScore;
  });

  std::vector<std::shared_ptr<Boid>> selected(boids.begin(),
                                              boids.begin() + selectNum);

  int nextId = 0;
  std::vector<AgentProps> mutated;
  while (mutated.size() < population) {
    for (auto &boid : selected) {
      AgentProps props;
      props.id = ++nextId;
      props.generationIndex = boid->getGenerationIndex() + 1;
      props.weights = mutateWeights(boid->getWeights());

      mutated.push_back(props);

      if (mutated.size() >= population) {
        break;
      }
    }
  }

  simulation.setBoids(mutated);
  simulation.resetFoodSources();
}

float Evolution::fitnessFunction(Boid &boid) {
  return static_cast<float>(boid.getNumFoodsEaten());
}

std::vector<float> Evolution::mutateWeights(std::vector<float> weights) {
  std::vector<float> results;
  for (auto &w : weights) {
    results.push_back(std::clamp(w + Util::randf(-0.1f, 0.1f), -1.f, 1.f));
  }
  return results;
}
