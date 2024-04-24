#pragma once

#include <filesystem>

class Screenshot {
	static const int frameInterval = 10;
	int currFrame;

	std::vector<sf::Texture> recordingFrameTextures;

	std::string basePath;
	std::string buildPath;

	int nextBuildNum;
	int nextFrameNum;

	int getNextBuildNum(std::string basePath);

	std::string getBuildPath(std::string basePath, int buildNum);
	std::string getFramePath(std::string buildPath, int frameNum);

	std::string pad(int n);
	std::string escapeRegex(std::string regex);

public:
	Screenshot(std::string screenshotDirPath);
	~Screenshot();

	bool isRecording;

	void capture(sf::RenderWindow& window);
	void startRecording(sf::RenderWindow& window);
	void step(sf::RenderWindow& window);
	void stopRecording();
};