#include "screenshot.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

Screenshot::Screenshot() : Screenshot(3) {}

Screenshot::Screenshot(int frameInterval)
    : frameInterval(frameInterval), buildNamer("screenies/", "build", "/"),
      frameNamer(buildNamer.curr(), "frame", "png") {}

void Screenshot::capture(sf::RenderWindow &window) {
  std::string buildPath = buildNamer.curr();
  std::string framePath = frameNamer.curr();

  if (!isBuildDirectoryCreated) {
    std::filesystem::create_directories(buildPath);
    isBuildDirectoryCreated = true;
  }

  sf::Texture texture;
  texture.create(window.getSize().x, window.getSize().y);
  texture.update(window);
  if (texture.copyToImage().saveToFile(framePath)) {
    std::cout << "Screenshot saved to " << framePath << std::endl;
  }

  frameNamer.next();
}

void Screenshot::toggleRecording() {
  if (isRecording) {
    stopRecording();
  } else {
    startRecording();
  }
}

void Screenshot::startRecording() {
  recordingFrameTextures.clear();
  isRecording = true;
}

void Screenshot::frameChanged(sf::RenderWindow &window) {
  if (isRecording) {
    if (--frameIntervalRemaining <= 0) {
      frameIntervalRemaining = frameInterval;

      sf::Texture texture;
      texture.create(window.getSize().x, window.getSize().y);
      texture.update(window);

      recordingFrameTextures.push_back(texture);
    }
  }
}

void Screenshot::stopRecording() {
  std::string buildPath = buildNamer.curr();

  if (!isBuildDirectoryCreated) {
    std::filesystem::create_directories(buildPath);
    isBuildDirectoryCreated = true;
  }

  for (sf::Texture frameTexture : recordingFrameTextures) {
    std::string framePath = frameNamer.curr();
    if (frameTexture.copyToImage().saveToFile(framePath)) {
      std::cout << "Screenshot saved to " << framePath << std::endl;
    }

    frameNamer.next();
  }

  recordingFrameTextures.clear();
  isRecording = false;
}

std::string Screenshot::pad(int n) {
  std::stringstream ss;
  ss << std::setw(4) << std::setfill('0') << n;
  return ss.str();
}

std::string Screenshot::escapeRegex(std::string regex) {
  // matches any characters that need to be escaped in RegEx
  // https://stackoverflow.com/a/40195721
  std::regex specialChars{R"([-[\]{}()*+?.,\^$|#\s\\])"};
  return std::regex_replace(regex, specialChars, R"(\$&)");
}