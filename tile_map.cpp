#include "tile_map.hpp"
#include "util.hpp"
#include <Eigen/Dense>

using Eigen::Vector2f;

TileMap::TileMap(unsigned int w, unsigned int h) : width(w), height(h) {
  // Make sure the size is a multiple of the tile size.
  assert(width % tileSize == 0);
  assert(height % tileSize == 0);

  unsigned int iw = width / tileSize;
  unsigned int ih = height / tileSize;

  tiles.reserve(iw * ih);
  for (unsigned int ix = 0; ix < iw; ++ix) {
    for (unsigned int iy = 0; iy < ih; ++iy) {
      Tile tile(ix, iy, ix * tileSize, iy * tileSize);

      if (ix == 0 || iy == 0 || ix == iw - 1 || iy == ih - 1) {
        tile.tileType = TileType::Wall;
      } else if (Util::randf() < 0.1) {
        tile.tileType = TileType::Wall;
      }

      tiles.push_back(tile);
    }
  }
}

Tile TileMap::getTile(unsigned int ix, unsigned int iy) {
  return tiles[ix * cols + iy];
}

Tile TileMap::getTileAt(float x, float y) {
  return tiles[floor(x / tileSize) * cols + floor(y / tileSize)];
}

std::vector<Tile> &TileMap::getTiles() { return tiles; }
