#pragma once

#include <filesystem>

class FileNamer {
	std::string basePath;
	std::string prefix;
	std::string extension;
	int index;

	int getInitialIndex();
	std::string getFilename(int i);
	std::string pad(int n);
	std::string escapeRegex(std::string regex);

public:
	FileNamer(std::string basePath, std::string prefix, std::string extension);
	std::string curr();
	int currIndex();
	std::string next();
	int nextIndex();
};

