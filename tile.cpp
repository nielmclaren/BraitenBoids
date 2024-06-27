#include "tile.hpp"

Tile::Tile(unsigned int ix, unsigned int iy, unsigned int x, unsigned int y)
    : tileType(TileType::Floor), ix(ix), iy(iy), x(x), y(y) {}

Tile::Tile(TileType tileType, unsigned int ix, unsigned int iy, unsigned int x,
           unsigned int y)
    : tileType(tileType), ix(ix), iy(iy), x(x), y(y) {}
