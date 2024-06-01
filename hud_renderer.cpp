#include "hud_renderer.hpp"

HudRenderer::HudRenderer(sf::RenderWindow &win)
    : window(win), generationIndex(0), stepCount(0), foodConsumed(0) {
  if (!bahnschriftRegularFont.loadFromFile("assets/bahnschrift.ttf")) {
    std::cout << "Error loading font." << std::endl;
  }

  hudTopLeftText.setPosition(8, 5);
  hudTopLeftText.setFont(bahnschriftRegularFont);
  hudTopLeftText.setString("Loading...");
  hudTopLeftText.setCharacterSize(16);
  hudTopLeftText.setFillColor(sf::Color(96, 96, 96));
}

void HudRenderer::draw() {
  std::string output = "Generation: " + std::to_string(generationIndex) + "\n" +
                       "Step: " + std::to_string(stepCount) + "\n" +
                       "Food consumed: " + std::to_string(foodConsumed) + "\n";
  hudTopLeftText.setString(output);
  window.draw(hudTopLeftText);
}

void HudRenderer::setGenerationIndex(unsigned int v) { generationIndex = v; }
void HudRenderer::setStepCount(unsigned int v) { stepCount = v; }
void HudRenderer::setFoodConsumed(unsigned int v) { foodConsumed = v; }