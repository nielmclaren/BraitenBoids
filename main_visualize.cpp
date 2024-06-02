#include "main_visualize.hpp"
#include <algorithm>

MainVisualize::MainVisualize(int argc, char *argv[])
    : window(sf::VideoMode(800, 800), "BraitenBoids"), simulation(800, 800),
      generationIndex(0), stepCount(0) {
  std::cout << "visualize command" << std::endl;

  // Seed the random number generator.
  srand(static_cast<unsigned>(time(0)));

  window.setKeyRepeatEnabled(false);
  window.setFramerateLimit(60);

  simRunner = SimRunner::create(simulation);
  simulation.registerEntityListener(simRunner);

  simRenderer = SimRenderer::create(simulation, window);
  simulation.registerEntityListener(simRenderer);

  HudRenderer hudRenderer(window);

  simRunner->resetAgents();
  simRunner->resetFoodSources();

  while (window.isOpen()) {
    sf::Time elapsed = clockwork.getElapsedTime();
    float elapsedSeconds = elapsed.asSeconds();
    clockwork.restart();

    handleEvent(window);
    simulation.setPlayerDirection(getPlayerInputDirection());
    simRunner->step(elapsedSeconds);
    hudRenderer.setGenerationIndex(generationIndex);
    hudRenderer.setStepCount(stepCount);
    hudRenderer.setFoodConsumed(SimRunner::numInitialFoodSources -
                                simulation.getNumFoodSources());

    window.clear(sf::Color::White);
    simRenderer->draw();
    hudRenderer.draw();
    window.display();

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
        simRunner->saveAgents("output/boids.json");
        evolutionLog.save("output/evolution_log.csv");
      }
      if (event.key.scancode == sf::Keyboard::Scan::L) {
        simRunner->loadAgents("output/boids.json");
        simRunner->resetFoodSources();
        evolutionLog.clear();

        generationIndex = getGenerationIndex(simulation);
        stepCount = 0;
      }
      if (event.key.scancode == sf::Keyboard::Scan::N) {
        simRunner->resetAgents();
        simRunner->resetFoodSources();
        evolutionLog.clear();

        generationIndex = getGenerationIndex(simulation);
        stepCount = 0;
      }
      if (event.key.scancode == sf::Keyboard::Scan::M) {
        reportGenerationFitness(simulation);
        evolutionLog.addEntry(simulation, generationIndex, stepCount);
        simRunner->selectAndMutate();
        simRunner->resetFoodSources();

        generationIndex = getGenerationIndex(simulation);
        stepCount = 0;
      }
      if (event.key.scancode == sf::Keyboard::Scan::F) {
        stepCount += simRunner->fastForward();

        reportGenerationFitness(simulation);
        evolutionLog.addEntry(simulation, generationIndex, stepCount);
        simRunner->selectAndMutate();
        simRunner->resetFoodSources();

        generationIndex = getGenerationIndex(simulation);
        stepCount = 0;
      }
      if (event.key.scancode == sf::Keyboard::Scan::W ||
          event.key.scancode == sf::Keyboard::Scan::A ||
          event.key.scancode == sf::Keyboard::Scan::S ||
          event.key.scancode == sf::Keyboard::Scan::D) {
        simRenderer->setDisplayAvatar(true);
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
  std::vector<AgentProps> propses = simulation.getAgents();
  for (auto &props : propses) {
    result = std::max(props.generationIndex, result);
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
  simRunner->reportGenerationFitness();
}
