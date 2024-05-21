#include "main_visualize.hpp"
#include "boid_marshaller.hpp"
#include "util.hpp"
#include <algorithm>

MainVisualize::MainVisualize(int argc, char *argv[])
    : window(sf::VideoMode(800, 800), "BraitenBoids"),
      simRenderer(simulation, window), stepCount(0), generationIndex(0) {
  std::cout << "visualize command" << std::endl;

  // Seed the random number generator.
  srand(static_cast<unsigned>(time(0)));

  simulation.init(static_cast<float>(window.getSize().x),
                  static_cast<float>(window.getSize().y));

  window.setKeyRepeatEnabled(false);
  window.setFramerateLimit(60);

  BoidMarshaller::loadRandomBoids(simulation);
  simulation.resetFoodSources();

  while (window.isOpen()) {
    sf::Time elapsed = clockwork.getElapsedTime();
    float elapsedSeconds = elapsed.asSeconds();
    clockwork.restart();

    handleEvent(window);
    simulation.setPlayerDirection(getPlayerInputDirection());
    simulation.step(elapsedSeconds);
    simRenderer.draw();

    screenshot.frameChanged(window);

    stepCount++;
  }
}

Vector2f MainVisualize::getPlayerInputDirection() {
  Vector2f direction(0, 0);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    direction.x() -= 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    direction.x() += 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    direction.y() -= 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    direction.y() += 1;
  }

  direction.normalize();
  return direction;
}

void MainVisualize::handleEvent(sf::RenderWindow &window) {
  sf::Event event;
  while (window.pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed: {
      window.close();
      break;
    }

    case sf::Event::KeyReleased: {
      if (event.key.scancode == sf::Keyboard::Scan::Escape) {
        window.close();
      }
      if (event.key.scancode == sf::Keyboard::Scan::R) {
        screenshot.capture(window);
      }
      if (event.key.scancode == sf::Keyboard::Scan::G) {
        screenshot.toggleRecording();
      }
      if (event.key.scancode == sf::Keyboard::Scan::V) {
        BoidMarshaller::save(simulation, "output/boids.csv");
        evolutionLog.save("output/evolution_log.csv");
      }
      if (event.key.scancode == sf::Keyboard::Scan::L) {
        BoidMarshaller::load(simulation, "output/boids.csv");
        simulation.resetFoodSources();
        generationIndex = 0;
        for (auto &boid : simulation.boids) {
          generationIndex =
              std::max(boid->getGenerationIndex(), generationIndex);
        }
        evolutionLog.clear();
      }
      if (event.key.scancode == sf::Keyboard::Scan::N) {
        BoidMarshaller::loadRandomBoids(simulation);
        simulation.resetFoodSources();

        stepCount = 0;
        generationIndex = 0;
        evolutionLog.clear();
      }
      if (event.key.scancode == sf::Keyboard::Scan::M) {
        reportGenerationFitness(simulation);
        logGeneration(simulation);
        selectAndMutate(simulation);
      }
      if (event.key.scancode == sf::Keyboard::Scan::F) {
        fastForward(simulation);
        reportGenerationFitness(simulation);
        logGeneration(simulation);
        selectAndMutate(simulation);
      }
      break;
    }

    default: {
    }
    }
  }
}

void MainVisualize::reportGenerationFitness(Simulation &simulation) {
  /*
  How many steps did it take to reach end conditions?
  What's the fitness spread?
  What's the highest and lowest fitness?
  What's the nn weight spread?
  */
  std::cout << "Generation: " << generationIndex << std::endl;
  std::cout << "\tSteps: " << stepCount << std::endl;

  std::cout << "\tFitness scores, weights: " << std::endl;
  std::vector<std::shared_ptr<Boid>> boids = simulation.boids;
  for (auto &boid : boids) {
    boid->fitnessScore = fitnessFunction(*boid);
  }
  sort(boids.begin(), boids.end(), [](const auto &lhs, const auto &rhs) {
    return lhs->fitnessScore > rhs->fitnessScore;
  });
  for (auto &boid : boids) {
    std::cout << "\t\t" << boid->fitnessScore << "\t";
    for (auto &w : boid->getWeights()) {
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

void MainVisualize::logGeneration(Simulation &simulation) {
  std::vector<std::shared_ptr<Boid>> boids = simulation.boids;
  for (auto &boid : boids) {
    boid->fitnessScore = fitnessFunction(*boid);
  }
  sort(boids.begin(), boids.end(), [](const auto &lhs, const auto &rhs) {
    return lhs->fitnessScore > rhs->fitnessScore;
  });

  int numFoodSourcesRemaining = static_cast<int>(simulation.foodSources.size());
  float foodConsumedPerStep =
      stepCount <= 0 ? 0
                     : static_cast<float>(Simulation::numInitialFoodSources -
                                          numFoodSourcesRemaining) /
                           static_cast<float>(stepCount);
  evolutionLog.addEntry(generationIndex, foodConsumedPerStep);
}

void MainVisualize::selectAndMutate(Simulation &simulation) {
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
  std::vector<BoidProps> mutated;
  while (mutated.size() < population) {
    for (auto &boid : selected) {
      BoidProps props;
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

  stepCount = 0;
  generationIndex++;
}

float MainVisualize::fitnessFunction(Boid &boid) {
  return static_cast<float>(boid.getNumFoodsEaten());
}

std::vector<float> MainVisualize::mutateWeights(std::vector<float> weights) {
  std::vector<float> results;
  for (auto &w : weights) {
    results.push_back(std::clamp(w + Util::randf(-0.1f, 0.1f), -1.f, 1.f));
  }
  return results;
}

void MainVisualize::fastForward(Simulation &simulation) {
  for (int i = 0; i < 10000; i++) {
    simulation.step(0.016f);
    stepCount++;

    if (simulation.foodSources.size() < 7) {
      break;
    }
  }
}
