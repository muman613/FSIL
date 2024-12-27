//
// Created by michael on 12/22/24.
//

#include "FileEntry.h"

#include <utility>
#include <exception>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#endif


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

std::filesystem::file_time_type FileEntry::getModificationTime() const {
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

std::filesystem::perms FileEntry::getPermissions() const {
    if (exists()) {
        return std::filesystem::status(filePath).permissions();
    }
    throw std::runtime_error("File does not exist");
}

std::filesystem::file_type FileEntry::getType() const {
    if (exists()) {
        return std::filesystem::status(filePath).type();
    }
    throw std::runtime_error("File does not exist");
}

std::filesystem::file_time_type FileEntry::getCreationTime() const {
#ifdef _WIN32
    return getWindowsCreationTime();
#else
    return getPOSIXCreationTime();
#endif
}

std::filesystem::file_time_type FileEntry::getLastAccessTime() const {
#ifdef _WIN32
    HANDLE fileHandle = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Unable to retrieve access time on Windows");
    }

    FILETIME lastAccessTime;
    if (!GetFileTime(fileHandle, nullptr, &lastAccessTime, nullptr)) {
        CloseHandle(fileHandle);
        throw std::runtime_error("Unable to retrieve access time on Windows");
    }

    CloseHandle(fileHandle);

    auto duration = std::chrono::nanoseconds((static_cast<uint64_t>(lastAccessTime.dwHighDateTime) << 32) | lastAccessTime.dwLowDateTime);
    return std::filesystem::file_time_type(std::chrono::duration_cast<std::filesystem::file_time_type::duration>(duration));
#else
    return getPOSIXAccessTime();
    // struct stat fileStat;
    // if (stat(filePath.c_str(), &fileStat) != 0) {
    //     throw std::runtime_error("Unable to retrieve access time on POSIX");
    // }
    //
    // auto timePoint = std::chrono::system_clock::from_time_t(fileStat.st_atime);
    // auto duration = timePoint.time_since_epoch();
    // return std::filesystem::file_time_type(std::chrono::duration_cast<std::filesystem::file_time_type::duration>(duration));
#endif
}


std::string FileEntry::fileTimeToString(const std::filesystem::file_time_type& time) {
    // Convert std::filesystem::file_time_type to system_clock::time_point
    auto systemTimePoint = std::chrono::system_clock::now() +
                           (time - std::filesystem::file_time_type::clock::now());

    // Convert to time_t for formatting
    auto systemTime = std::chrono::system_clock::to_time_t(systemTimePoint);

    // Format the time to a readable string
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&systemTime), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

#ifdef _WIN32
std::filesystem::file_time_type FileEntry::getWindowsCreationTime() const {
    HANDLE fileHandle = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Unable to retrieve creation time on Windows");
    }

    FILETIME creationTime;
    if (!GetFileTime(fileHandle, &creationTime, nullptr, nullptr)) {
        CloseHandle(fileHandle);
        throw std::runtime_error("Unable to retrieve creation time on Windows");
    }

    CloseHandle(fileHandle);

    auto duration = std::chrono::nanoseconds((static_cast<uint64_t>(creationTime.dwHighDateTime) << 32) | creationTime.dwLowDateTime);
    return std::filesystem::file_time_type(std::chrono::duration_cast<std::filesystem::file_time_type::duration>(duration));
}

std::filesystem::file_time_type FileEntry::getWindowsAccessTime() const {
    return {};
}

#else
std::filesystem::file_time_type FileEntry::getPOSIXCreationTime() const {
    struct stat fileStat{};
    if (stat(filePath.c_str(), &fileStat) != 0) {
        throw std::runtime_error("Unable to retrieve creation time on POSIX");
    }

    auto timePoint = std::chrono::system_clock::from_time_t(fileStat.st_ctime);
    auto duration = timePoint.time_since_epoch();
    return std::filesystem::file_time_type(std::chrono::duration_cast<std::filesystem::file_time_type::duration>(duration));
}

std::filesystem::file_time_type FileEntry::getPOSIXAccessTime() const {
    struct stat fileStat{};
    if (stat(filePath.c_str(), &fileStat) != 0) {
        throw std::runtime_error("Unable to retrieve creation time on POSIX");
    }

    auto timePoint = std::chrono::system_clock::from_time_t(fileStat.st_atime);
    auto duration = timePoint.time_since_epoch();
    return std::filesystem::file_time_type(std::chrono::duration_cast<std::filesystem::file_time_type::duration>(duration));
}

#endif

FileEntryPtr FileEntry::newEntry(const std::filesystem::path &path) {
    return std::make_unique<FileEntry>(path);
}
