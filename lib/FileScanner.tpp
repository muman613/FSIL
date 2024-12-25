//
// FileScanner.tpp
// Created by michael on 12/22/24.
//

#ifndef FILESCANNER_TPP
#define FILESCANNER_TPP

#include <filesystem>
#include <vector>

template <typename Callable>
void FileScanner::scan(const std::filesystem::path& directory, FileEntryVec& entries, Callable filter, bool recursive) {
    if (recursive) {
        scanImpl<std::filesystem::recursive_directory_iterator>(directory, entries, filter);
    } else {
        scanImpl<std::filesystem::directory_iterator>(directory, entries, filter);
    }
}

template <typename IteratorType, typename Callable>
void FileScanner::scanImpl(const std::filesystem::path& directory, FileEntryVec& entries, Callable filter) {
    for (const auto& entry : IteratorType(directory)) {
        if (entry.is_regular_file()) {
            const auto& path = entry.path();
            if (filter(path)) {
                entries.push_back(FileEntry::newEntry(path));
            }
        }
    }
}

#endif // FILESCANNER_TPP
