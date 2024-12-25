//
// FileScanner.cpp
// Created by michael on 12/22/24.
//

#include "FileScanner.h"
#include <stdexcept>

// Singleton instance accessor
FileScanner& FileScanner::getInstance() {
    static FileScanner instance;
    return instance;
}

// Private constructor
FileScanner::FileScanner() = default;

// Destructor
FileScanner::~FileScanner() = default;

// Scan using regex patterns (optionally recursive)
void FileScanner::scan(const std::filesystem::path& directory, FileEntryVec& entries, const std::vector<std::string>& patterns, bool recursive) {
    std::vector<std::regex> regexList;
    for (const auto& pattern : patterns) {
        regexList.emplace_back(pattern);
    }

    auto filter = [&regexList](const std::filesystem::path& path) {
        const std::string filename = path.filename().string();
        for (const auto& regex : regexList) {
            if (std::regex_match(filename, regex)) {
                return true;
            }
        }
        return false;
    };

    if (recursive) {
        scanImpl<std::filesystem::recursive_directory_iterator>(directory, entries, filter);
    } else {
        scanImpl<std::filesystem::directory_iterator>(directory, entries, filter);
    }
}
