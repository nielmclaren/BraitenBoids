#include <filesystem>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "screenshot.hpp"

namespace fs = std::filesystem;

Screenshot::Screenshot() :
        buildNamer("screenies/", "build", "/"),
        frameNamer(buildNamer.curr(), "frame", "png"),
        isRecording(false) {
}

void Screenshot::capture(sf::RenderWindow& window) {
    std::string buildPath = buildNamer.curr();
    std::cout << "buildPath=" << buildPath << std::endl;

    std::string framePath = frameNamer.curr();
    std::cout << "framePath=" << framePath << std::endl;

    if (frameNamer.currIndex() == 0) {
        std::filesystem::create_directories(buildPath);
    }

    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);
    if (texture.copyToImage().saveToFile(framePath)) {
        std::cout << "screenshot saved to " << framePath << std::endl;
    }

    frameNamer.next();
}

void Screenshot::toggleRecording() {
    if (isRecording) {
        stopRecording();
    }
    else {
        startRecording();
    }
}

void Screenshot::startRecording() {
    recordingFrameTextures.clear();
    isRecording = true;
}

void Screenshot::step(sf::RenderWindow& window) {
    if (isRecording) {
        sf::Texture texture;
        texture.create(window.getSize().x, window.getSize().y);
        texture.update(window);

        recordingFrameTextures.push_back(texture);
    }
}

void Screenshot::stopRecording() {
    std::string buildPath = buildNamer.curr();
    std::cout << "buildPath=" << buildPath << std::endl;

    for (sf::Texture frameTexture : recordingFrameTextures) {
        std::string framePath = frameNamer.curr();
        std::cout << "framePath=" << framePath << std::endl;

        if (frameNamer.currIndex() == 0) {
            std::filesystem::create_directories(buildPath);
        }

        if (frameTexture.copyToImage().saveToFile(framePath)) {
            std::cout << "screenshot saved to " << framePath << std::endl;
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
    std::regex specialChars{ R"([-[\]{}()*+?.,\^$|#\s\\])" };
    return std::regex_replace(regex, specialChars, R"(\$&)");
}