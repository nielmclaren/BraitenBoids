#pragma once

#include "tile.hpp"
#include <vector>

class TileMap {
  unsigned int cols;
  unsigned int rows;
  unsigned int width;
  unsigned int height;
  std::vector<Tile> tiles;

public:
  static const unsigned int tileSize = 32;

  TileMap(unsigned int w, unsigned int h);

  unsigned int getCols() const;
  unsigned int getRows() const;
  unsigned int getWidth() const;
  unsigned int getHeight() const;

  Tile getTile(unsigned int ix, unsigned int iy) const;
  Tile getTileAt(float x, float y) const;
  const std::vector<Tile> &getTiles() const;
};
