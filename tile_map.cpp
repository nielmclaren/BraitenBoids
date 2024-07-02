#include "tile_map.hpp"
#include "util.hpp"
#include <Eigen/Dense>
#include <iostream>

using Eigen::Vector2f;

TileMap::TileMap(unsigned int w, unsigned int h) : width(w), height(h) {
  // Make sure the size is a multiple of the tile size.
  assert(width % tileSize == 0);
  assert(height % tileSize == 0);

  cols = width / tileSize;
  rows = height / tileSize;

  tiles.reserve(cols * rows);
  for (unsigned int ix = 0; ix < cols; ++ix) {
    for (unsigned int iy = 0; iy < rows; ++iy) {
      Tile tile(ix, iy, ix * tileSize, iy * tileSize);

      if (ix == 0 || iy == 0 || ix == cols - 1 || iy == rows - 1) {
        tile.tileType = TileType::Wall;
      } else if (Util::randf() < 0.1) {
        tile.tileType = TileType::Wall;
      }

      tiles.push_back(tile);
    }
  }
}

unsigned int TileMap::getCols() const { return cols; }
unsigned int TileMap::getRows() const { return rows; }
unsigned int TileMap::getWidth() const { return width; }
unsigned int TileMap::getHeight() const { return height; }

Tile TileMap::getTile(unsigned int ix, unsigned int iy) const {
  return tiles[ix * cols + iy];
}

Tile TileMap::getTileAt(float x, float y) const {
  return tiles[floor(x / tileSize) * cols + floor(y / tileSize)];
}

const std::vector<Tile> &TileMap::getTiles() const { return tiles; }
