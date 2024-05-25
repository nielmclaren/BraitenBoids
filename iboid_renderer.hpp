#pragma once

#include "iagent.hpp"
#include "iworld_state.hpp"
#include <SFML/Graphics.hpp>

class IBoidRenderer {
public:
  virtual ~IBoidRenderer() {}
  virtual unsigned int getBoidId() const = 0;
  virtual void draw(IWorldState &worldState, sf::RenderWindow &window) = 0;
};