#include <filesystem>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "file_namer.hpp"

namespace fs = std::filesystem;

FileNamer::FileNamer(std::string basePath, std::string prefix, std::string extension) :
        basePath(basePath),
        prefix(prefix),
        extension(extension),
        index(getInitialIndex()) {}

std::string FileNamer::curr() {
    return getFilename(index);
}

int FileNamer::currIndex() {
    return index;
}

std::string FileNamer::next() {
    index++;
    return getFilename(index);
}

int FileNamer::nextIndex() {
    index++;
    return index;
}

int FileNamer::getInitialIndex() {
    if (!fs::exists(basePath)) {
        return 0;
    }

    std::stringstream ss;
    ss << escapeRegex(basePath);
    if (basePath.back() != '/') ss << '/';
    ss << escapeRegex(prefix) << R"((\d+))";
    std::string regex = ss.str();
    std::regex re(regex, std::regex_constants::ECMAScript);
    std::smatch matches;

    int highestIndex = -1;
    for (const auto& entry : fs::directory_iterator(basePath)) {
        std::string path = entry.path().string();
        std::cout << "regex=" << regex << "\t" << "path=" << path << std::endl;
        if (std::regex_search(path, matches, re)) {
            int index = stoi(matches[1]);
            std::cout << "MATCH " << index << std::endl;
            if (index > highestIndex) {
                highestIndex = index;
            }
        }
    }
    std::cout << "highest index = " << highestIndex << std::endl;
    return highestIndex + 1;
}

std::string FileNamer::getFilename(int i) {
    std::stringstream ss;
    ss << basePath;
    if (basePath.back() != '/') ss << '/';
    ss << prefix << pad(index);
    if (extension == "") return ss.str();
    if (extension == "/") {
        ss << "/";
        return ss.str();
    }
    ss << "." << extension;
    return ss.str();
}

std::string FileNamer::pad(int n) {
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << n;
    return ss.str();
}

std::string FileNamer::escapeRegex(std::string regex) {
    // matches any characters that need to be escaped in RegEx
    // https://stackoverflow.com/a/40195721
    std::regex specialChars{ R"([-[\]{}()*+?.,\^$|#\s\\])" };
    return std::regex_replace(regex, specialChars, R"(\$&)");
}