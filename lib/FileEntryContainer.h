//
// Created by michael on 12/24/24.
//

#ifndef FILEENTRYCONTAINER_H
#define FILEENTRYCONTAINER_H

#include "FileEntry.h"
#include <filesystem>
#include <functional>

class FileEntryContainer {
public:
     FileEntryContainer() = default;
     FileEntryContainer(FileEntryContainer&& other) noexcept ;
     FileEntryContainer(std::initializer_list<FileEntry> entries);
//
     FileEntryPtr &operator[](int index);

     void append(const FileEntry& entry);
     void append(const std::filesystem::path& path);
     FileEntry& operator[](std::size_t index) const;
     size_t size() const;

     void sortFileEntriesAlphabetically();

     bool foreach(const std::function<bool(const FileEntry&)>& callback) const;
//
private:
     FileEntryVec fileEntries;
};



#endif //FILEENTRYCONTAINER_H
