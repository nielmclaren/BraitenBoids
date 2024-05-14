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
  SimRenderer simRenderer;
  unsigned int stepCount;
  unsigned int generationIndex;
  EvolutionLog evolutionLog;

  Screenshot screenshot;

  Vector2f getPlayerInputDirection();
  void handleEvent(sf::RenderWindow &window);

  void randomBoids(Simulation &simulation);
  void load(Simulation &simulation);
  void save(Simulation &simulation);
  void reportGenerationFitness(Simulation &simulation);
  void logGeneration(Simulation &simulation);
  void selectAndMutate(Simulation &simulation);
  float fitnessFunction(Boid &boid);
  std::vector<float> mutateWeights(std::vector<float> input);
  void fastForward(Simulation &simulation);

  float randf();

public:
  MainVisualize(int argc, char *argv[]);
};
