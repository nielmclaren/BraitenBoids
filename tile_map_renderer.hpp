#pragma once

#include "tile_map.hpp"
#include "tile_renderer.hpp"
#include <SFML/Graphics.hpp>

class TileMapRenderer {
public:
  const TileMap &tileMap;
  std::vector<TileRenderer> tileRenderers;

  TileMapRenderer(const TileMap &tileMap);

  void draw(sf::RenderWindow &window);
};