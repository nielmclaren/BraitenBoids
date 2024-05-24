#include "main_visualize.hpp"
#include "boid_marshaller.hpp"
#include "evolution.hpp"
#include <algorithm>

MainVisualize::MainVisualize(int argc, char *argv[])
    : window(sf::VideoMode(800, 800), "BraitenBoids"),
      simulation(static_cast<float>(window.getSize().x),
                 static_cast<float>(window.getSize().y)),
      stepCount(0) {
  std::cout << "visualize command" << std::endl;

  // Seed the random number generator.
  srand(static_cast<unsigned>(time(0)));

  window.setKeyRepeatEnabled(false);
  window.setFramerateLimit(60);

  std::shared_ptr<SimRenderer> simRenderer =
      SimRenderer::create(simulation, window);
  simulation.registerEntityListener(simRenderer);

  simulation.resetBoids();
  simulation.resetFoodSources();

  while (window.isOpen()) {
    sf::Time elapsed = clockwork.getElapsedTime();
    float elapsedSeconds = elapsed.asSeconds();
    clockwork.restart();

    handleEvent(window);
    simulation.setPlayerDirection(getPlayerInputDirection());
    simulation.step(elapsedSeconds);
    simRenderer->draw();

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
        BoidMarshaller::save(simulation, "output/boids.json");
        evolutionLog.save("output/evolution_log.csv");
      }
      if (event.key.scancode == sf::Keyboard::Scan::L) {
        BoidMarshaller::load(simulation, "output/boids.json");
        simulation.resetFoodSources();
        evolutionLog.clear();
      }
      if (event.key.scancode == sf::Keyboard::Scan::N) {
        simulation.resetBoids();
        simulation.resetFoodSources();

        stepCount = 0;
        evolutionLog.clear();
      }
      if (event.key.scancode == sf::Keyboard::Scan::M) {
        reportGenerationFitness(simulation);
        evolutionLog.addEntry(simulation, getGenerationIndex(simulation),
                              stepCount);
        Evolution::selectAndMutate(simulation);

        stepCount = 0;
      }
      if (event.key.scancode == sf::Keyboard::Scan::F) {
        fastForward(simulation);
        reportGenerationFitness(simulation);
        evolutionLog.addEntry(simulation, getGenerationIndex(simulation),
                              stepCount);
        Evolution::selectAndMutate(simulation);

        stepCount = 0;
      }
      break;
    }

    default: {
    }
    }
  }
}

unsigned int MainVisualize::getGenerationIndex(Simulation &simulation) {
  unsigned int result = 0;
  for (auto &boid : simulation.boids) {
    result = std::max(boid->getGenerationIndex(), result);
  }
  return result;
}

void MainVisualize::reportGenerationFitness(Simulation &simulation) {
  /*
  How many steps did it take to reach end conditions?
  What's the fitness spread?
  What's the highest and lowest fitness?
  What's the nn weight spread?
  */

  std::cout << "Generation: " << getGenerationIndex(simulation) << std::endl;
  std::cout << "\tSteps: " << stepCount << std::endl;

  std::cout << "\tFitness scores, weights: " << std::endl;
  std::vector<std::shared_ptr<Boid>> boids = simulation.boids;
  for (auto &boid : boids) {
    boid->fitnessScore = Evolution::fitnessFunction(*boid);
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

void MainVisualize::fastForward(Simulation &simulation) {
  for (int i = 0; i < 10000; i++) {
    simulation.step(0.016f);
    stepCount++;

    if (simulation.foodSources.size() < 7) {
      break;
    }
  }
}
