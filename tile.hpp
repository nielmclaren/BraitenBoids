#pragma once

#include "constants.hpp"

class Tile {
public:
  TileType tileType;

  unsigned int ix;
  unsigned int iy;
  unsigned int x;
  unsigned int y;

  Tile(unsigned int ix, unsigned int iy, unsigned int x, unsigned int y);
  Tile(TileType tileType, unsigned int ix, unsigned int iy, unsigned int x,
       unsigned int y);
};
