//
// Created by michael on 12/22/24.
//

#ifndef FILEENTRY_H
#define FILEENTRY_H

#include <filesystem>
#include <vector>

class FileEntry;

using FileEntryPtr = std::unique_ptr<FileEntry>;
using FileEntryVec = std::vector<FileEntryPtr>;


class FileEntry {
public:
    FileEntry() = default;
    explicit FileEntry(std::filesystem::path  path);
    FileEntry(const FileEntry& other) = default;
    FileEntry(FileEntry&& other) = default;

    [[nodiscard]] bool exists() const;

    void setPath(const std::filesystem::path& path);
    [[nodiscard]] std::filesystem::path getPath() const;

    [[nodiscard]] uintmax_t getSize() const;

    [[nodiscard]] std::filesystem::path getName() const;
    [[nodiscard]] std::filesystem::path getExtension() const;

    [[nodiscard]] std::filesystem::perms getPermissions() const;
    [[nodiscard]] std::filesystem::file_type getType() const;
    [[nodiscard]] std::filesystem::file_time_type getModificationTime() const;
    [[nodiscard]] std::filesystem::file_time_type getCreationTime() const;
    [[nodiscard]] std::filesystem::file_time_type getLastAccessTime() const;

    [[nodiscard]] static std::string fileTimeToString(const std::filesystem::file_time_type& time);

    [[nodiscard]] std::string getContent() const;
    [[nodiscard]] std::vector<std::string> getLines() const;

    static FileEntryPtr newEntry(const std::filesystem::path& path);

private:
    std::filesystem::path filePath;

#ifdef _WIN32
    [[nodiscard]] std::filesystem::file_time_type getWindowsCreationTime() const;
    [[nodiscard]] std::filesystem::file_time_type getWindowsCreationTime() const;
#else
    [[nodiscard]] std::filesystem::file_time_type getPOSIXCreationTime() const;
    [[nodiscard]] std::filesystem::file_time_type getPOSIXAccessTime() const;
#endif

};



#endif //FILEENTRY_H
