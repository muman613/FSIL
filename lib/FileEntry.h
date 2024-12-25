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

    [[nodiscard]] std::filesystem::file_time_type getTime() const;
    [[nodiscard]] std::filesystem::path getName() const;
    [[nodiscard]] std::filesystem::path getExtension() const;

    [[nodiscard]] std::string getContent() const;
    [[nodiscard]] std::vector<std::string> getLines() const;

    static FileEntryPtr newEntry(const std::filesystem::path& path);

private:
    std::filesystem::path filePath;
};



#endif //FILEENTRY_H
