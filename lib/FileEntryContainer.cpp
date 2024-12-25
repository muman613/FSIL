//
// Created by michael on 12/24/24.
//

#include "FileEntryContainer.h"

#include <boost/test/utils/runtime/modifier.hpp>

FileEntryContainer::FileEntryContainer(FileEntryContainer &&other) noexcept {
    fileEntries = std::move(other.fileEntries);
}
//
FileEntryContainer::FileEntryContainer(std::initializer_list<FileEntry> entries) {
    for (auto &entry : entries) {
        fileEntries.emplace_back(FileEntry::newEntry(entry.getPath()));
    }
}

FileEntry &FileEntryContainer::operator[](size_t index) const {
    return *fileEntries[index].get();
}

bool FileEntryContainer::foreach(const std::function<bool(const FileEntry &)> &callback) const {
    for (const auto &entry : fileEntries ) {
        if (!callback(*entry)) {
            return false;
        }
    }

    return true;
}

size_t FileEntryContainer::size() const {
    return fileEntries.size();
}

void FileEntryContainer::append(const std::filesystem::path& path) {
    fileEntries.emplace_back(FileEntry::newEntry(path));
}

void FileEntryContainer::append(const FileEntry &entry) {
    fileEntries.emplace_back(FileEntry::newEntry(entry.getPath()));
}
