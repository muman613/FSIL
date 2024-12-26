//
// FileScanner.h
// Created by michael on 12/22/24.
//

#ifndef FILESCANNER_H
#define FILESCANNER_H

#include "FileEntry.h"
#include "FileEntryContainer.h"
#include <filesystem>
#include <regex>
#include <functional>
#include <vector>
#include <memory>

// FileScanner Singleton Class
class FileScanner {
public:
    // Access the singleton instance
    static FileScanner& getInstance();

    // Delete copy and move constructors
    FileScanner(const FileScanner&) = delete;
    FileScanner(FileScanner&&) = delete;
    FileScanner& operator=(const FileScanner&) = delete;
    FileScanner& operator=(FileScanner&&) = delete;

    // Scan with regex patterns (optionally recursive)
    void scan(const std::filesystem::path& directory, FileEntryVec& entries, const std::vector<std::string>& patterns, bool recursive = false);
    void scan(const std::filesystem::path& directory, FileEntryContainer& entries, const std::vector<std::string>& patterns, bool recursive = false);

    // Scan with a callable filter (optionally recursive)
    template <typename Callable>
    void scan(const std::filesystem::path& directory, FileEntryVec& entries, Callable filter, bool recursive = false);

    template <typename Callable>
    void scan(const std::filesystem::path& directory, FileEntryContainer& entries, Callable filter, bool recursive = false);

private:
    // Private constructor for singleton
    FileScanner();
    ~FileScanner();

    // Helper for recursive or non-recursive scanning
    template <typename IteratorType, typename Callable>
    void scanImpl(const std::filesystem::path& directory, FileEntryVec& entries, Callable filter);
    template <typename IteratorType, typename Callable>
    void scanImpl(const std::filesystem::path& directory, FileEntryContainer& entries, Callable filter);
};

#include "FileScanner.tpp" // Include template implementation

#endif // FILESCANNER_H
