//
// Created by michael on 12/22/24.
//

#include "FileEntry.h"

#include <utility>
#include <exception>
#include <fstream>

FileEntry::FileEntry(std::filesystem::path path) : filePath(std::move(path)) {

}

bool FileEntry::exists() const {
    if (!filePath.empty()) {
        return std::filesystem::exists(filePath);
    }

    return false;
}

void FileEntry::setPath(const std::filesystem::path &path) {
    filePath = path;
}

uintmax_t FileEntry::getSize() const {
    if (!filePath.empty()) {
        return std::filesystem::file_size(filePath);
    }
    throw std::runtime_error("File does not exist");
}

std::filesystem::file_time_type FileEntry::getTime() const {
    if (std::filesystem::exists(filePath)) {
        return std::filesystem::last_write_time(filePath);
    }
    throw std::runtime_error("File does not exist");
}

std::filesystem::path FileEntry::getPath() const {
    return filePath;
}

std::filesystem::path FileEntry::getName() const {
    if (std::filesystem::exists(filePath)) {
        return std::filesystem::path(filePath.filename());
    }
    throw std::runtime_error("File does not exist");
}

std::filesystem::path FileEntry::getExtension() const {
    if (std::filesystem::exists(filePath)) {
        return std::filesystem::path(filePath.extension());
    }
    throw std::runtime_error("File does not exist");
}

std::string FileEntry::getContent() const {
    if (std::filesystem::exists(filePath)) {
        std::ifstream file(filePath, std::ios::in | std::ios::binary); // Open file in binary mode
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filePath.string());
        }

        std::ostringstream contents;
        contents << file.rdbuf(); // Read file contents into stringstream
        file.close(); // Close file after reading

        return contents.str(); // Return the string
    }
    throw std::runtime_error("File does not exist");
}

std::vector<std::string> FileEntry::getLines() const {
    std::vector<std::string> lines;
    std::string content = getContent();
    std::istringstream contentStream(content);
    std::string line;

    while (std::getline(contentStream, line)) {
        lines.push_back(line);
    }

    return lines;
}

FileEntryPtr FileEntry::newEntry(const std::filesystem::path &path) {
    return std::make_unique<FileEntry>(path);
}
