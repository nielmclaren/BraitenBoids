#pragma once

#include <filesystem>
#include <SFML/Graphics.hpp>
#include "file_namer.hpp"

class Screenshot {
	FileNamer buildNamer;
	FileNamer frameNamer;

	bool isRecording;

	std::vector<sf::Texture> recordingFrameTextures;

	std::string pad(int n);
	std::string escapeRegex(std::string regex);

public:
	Screenshot();

	void capture(sf::RenderWindow& window);

	void toggleRecording();
	void startRecording();
	void step(sf::RenderWindow& window);
	void stopRecording();
};