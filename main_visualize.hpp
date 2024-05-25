#pragma once

#include "evolution_log.hpp"
#include "screenshot.hpp"
#include "sim_renderer.hpp"
#include "simulation.hpp"
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

using Eigen::Vector2f;

class MainVisualize {
  sf::Clock clockwork;

  sf::RenderWindow window;
  Simulation simulation;

  unsigned int stepCount;
  EvolutionLog evolutionLog;

  Screenshot screenshot;

  Vector2f getPlayerInputDirection();
  void handleEvent(sf::RenderWindow &window);

  unsigned int getGenerationIndex(Simulation &simulation);
  void reportGenerationFitness(Simulation &simulation);
  unsigned int fastForward(Simulation &simulation);

public:
  MainVisualize(int argc, char *argv[]);
};
