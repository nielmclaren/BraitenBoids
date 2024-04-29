#pragma once

#include <filesystem>
#include <SFML/Graphics.hpp>
#include "file_namer.hpp"

class Screenshot {
	FileNamer buildNamer;
	FileNamer frameNamer;

	bool isRecording = false;
	bool isBuildDirectoryCreated = false;

	// When recording, only capture one in every x frames.
	int frameInterval;
	int frameIntervalRemaining = 0;

	std::vector<sf::Texture> recordingFrameTextures;

	std::string pad(int n);
	std::string escapeRegex(std::string regex);

public:
	Screenshot();
	Screenshot(int frameInterval);

	void capture(sf::RenderWindow& window);

	void toggleRecording();
	void startRecording();
	void frameChanged(sf::RenderWindow& window);
	void stopRecording();
};