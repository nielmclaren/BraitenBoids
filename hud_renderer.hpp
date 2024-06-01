#pragma once

#include "simulation.hpp"
#include <SFML/Graphics.hpp>

class HudRenderer {
  sf::RenderWindow &window;

  sf::Font bahnschriftRegularFont;
  sf::Text hudTopLeftText;

  unsigned int generationIndex;
  unsigned int stepCount;
  unsigned int foodConsumed;

public:
  HudRenderer(sf::RenderWindow &win);

  void draw();
  void setGenerationIndex(unsigned int generationIndex);
  void setStepCount(unsigned int stepCount);
  void setFoodConsumed(unsigned int foodConsumed);
};
