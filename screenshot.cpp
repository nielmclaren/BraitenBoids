#include <filesystem>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "screenshot.hpp"

namespace fs = std::filesystem;

Screenshot::Screenshot(std::string basePathArg) {
    basePath = basePathArg;
    isRecording = false;
}

Screenshot::~Screenshot() {}

void Screenshot::capture(sf::RenderWindow& window) {
    std::string framePath = getFramePath(buildPath, nextFrameNum);
    std::cout << "framePath=" << framePath << std::endl;

    if (nextFrameNum == 0) {
        std::filesystem::create_directories(buildPath);
    }

    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);
    if (texture.copyToImage().saveToFile(framePath)) {
        std::cout << "screenshot saved to " << framePath << std::endl;
    }

    nextFrameNum++;
}

void Screenshot::startRecording(sf::RenderWindow& window) {
    recordingFrameTextures.clear();
    isRecording = true;
}

void Screenshot::step(sf::RenderWindow& window) {
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);

    recordingFrameTextures.push_back(texture);
}

void Screenshot::stopRecording() {
    nextBuildNum = getNextBuildNum(basePath);
    buildPath = getBuildPath(basePath, nextBuildNum);
    nextFrameNum = 0;

    std::cout << "Saving screenshots to " << buildPath << std::endl;

    for (sf::Texture frameTexture : recordingFrameTextures) {
        std::string framePath = getFramePath(buildPath, nextFrameNum);

        if (nextFrameNum == 0) {
            std::filesystem::create_directories(buildPath);
        }

        if (frameTexture.copyToImage().saveToFile(framePath)) {
            std::cout << "screenshot saved to " << framePath << std::endl;
        }

        nextFrameNum++;
    }

    recordingFrameTextures.clear();
    isRecording = false;
}

int Screenshot::getNextBuildNum(std::string basePath) {
    std::stringstream ss;
    ss << escapeRegex(basePath) << R"(\\build(\d+))";
    std::string regex = ss.str();
    std::regex re(regex, std::regex_constants::ECMAScript);
    std::smatch matches;

    int highestBuildNum = -1;
    for (const auto& entry : fs::directory_iterator(basePath)) {
        std::string path = entry.path().string();
        std::cout << "regex=" << regex << "\t" << "path=" << path << std::endl;
        if (std::regex_search(path, matches, re)) {
            int buildNum = stoi(matches[1]);
            std::cout << "MATCH" << buildNum << std::endl;
            if (buildNum > highestBuildNum) {
                highestBuildNum = buildNum;
            }
        }
    }
    return highestBuildNum + 1;
}

std::string Screenshot::getBuildPath(std::string basePath, int buildNum) {
    std::stringstream ss;
    ss << basePath << "\\build" << pad(buildNum);
    return ss.str();
}

std::string Screenshot::getFramePath(std::string buildPath, int frameNum) {
    std::stringstream ss;
    ss << buildPath << "\\frame" << pad(frameNum) << ".png";
    return ss.str();
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