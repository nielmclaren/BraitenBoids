#pragma once

#include "tile.hpp"
#include <SFML/Graphics.hpp>

class TileRenderer {
public:
  const Tile &tile;
  sf::RectangleShape tileShape;

  TileRenderer(const Tile &tile);

  void draw(sf::RenderWindow &window);
};