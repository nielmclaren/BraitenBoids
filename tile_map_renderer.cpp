#include "tile_map_renderer.hpp"
#include "tile_map.hpp"
#include "util.hpp"
#include <iostream>

TileMapRenderer::TileMapRenderer(const TileMap &tileMap) : tileMap(tileMap) {
  unsigned int cols = tileMap.getCols();
  unsigned int rows = tileMap.getRows();

  tileRenderers.reserve(cols * rows);
  // TODO: Get an iterator from TileMap instead?
  for (unsigned int ix = 0; ix < cols; ++ix) {
    for (unsigned int iy = 0; iy < rows; ++iy) {
      TileRenderer tileRenderer(tileMap.getTile(ix, iy));
      tileRenderers.push_back(tileRenderer);
    }
  }
}

void TileMapRenderer::draw(sf::RenderWindow &window) {
  unsigned int cols = tileMap.getCols();
  unsigned int rows = tileMap.getRows();

  for (unsigned int ix = 0; ix < cols; ++ix) {
    for (unsigned int iy = 0; iy < rows; ++iy) {
      tileRenderers[ix * rows + iy].draw(window);
    }
  }
}
