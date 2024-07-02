#include "tile_renderer.hpp"
#include "tile_map.hpp"
#include "util.hpp"

TileRenderer::TileRenderer(const Tile &tile)
    : tile(tile),
      tileShape(sf::Vector2f(TileMap::tileSize, TileMap::tileSize)) {
  tileShape.setPosition(static_cast<float>(tile.x), static_cast<float>(tile.y));
  tileShape.setOutlineColor(sf::Color(224, 224, 224));
  tileShape.setOutlineThickness(1);

  switch (tile.tileType) {
  case TileType::Wall:
    tileShape.setFillColor(sf::Color(240, 240, 240));
    break;
  case TileType::Floor:
    tileShape.setFillColor(sf::Color(255, 255, 255));
    break;
  default:
    throw "Unexpected tile type.";
  }
}

void TileRenderer::draw(sf::RenderWindow &window) { window.draw(tileShape); }
