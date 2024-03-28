#pragma once

#include <filesystem>

class Screenshot {
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

	void capture(sf::RenderWindow& window);
};