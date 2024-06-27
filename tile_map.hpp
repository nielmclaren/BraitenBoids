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

  Tile getTile(unsigned int ix, unsigned int iy);
  Tile getTileAt(float x, float y);
  std::vector<Tile> &getTiles();
};
