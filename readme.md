# File Scanning and Information Library

## Overview

`fileentries` is a lightweight C++17 library for efficient file management, scanning, and processing. It provides classes and utilities to encapsulate file operations, scan directories with customizable filters, and manage collections of files. The library is designed to simplify common file handling tasks in modern C++ applications.

---

## Features

1. **`FileEntry`**:
    - Encapsulates operations on a single file.
    - Provides methods to query file properties such as size, name, extension, last modified time, and content.
    - Includes functionality to read file contents as a string or split them into lines.

2. **`FileScanner`**:
    - A singleton class for directory scanning.
    - Supports recursive and non-recursive directory traversal.
    - Allows file filtering based on regular expressions or custom predicates.
    - Outputs results as a vector of `FileEntry` objects.

3. **`FileEntryContainer`**:
    - Manages a collection of `FileEntry` objects.
    - Supports appending individual files or paths.
    - Provides random access to file entries via `operator[]`.
    - Iterates over entries using a customizable `foreach` callback.

---

## Classes Overview

### `FileEntry`

The `FileEntry` class encapsulates file-level operations and properties.

**Key Methods**:
- `bool exists() const`: Checks if the file exists.
- `void setPath(const std::filesystem::path& path)`: Sets the file's path.
- `std::filesystem::path getPath() const`: Gets the file's path.
- `uintmax_t getSize() const`: Returns the file's size in bytes.
- `std::filesystem::file_time_type getTime() const`: Gets the file's last modified time.
- `std::filesystem::path getName() const`: Returns the file's name.
- `std::filesystem::path getExtension() const`: Returns the file's extension.
- `std::string getContent() const`: Reads the entire file into a string.
- `std::vector<std::string> getLines() const`: Reads the file and splits it into lines.

### `FileScanner`

The `FileScanner` class scans directories for files and provides filtering mechanisms.

**Key Methods**:
- `void scan(const std::filesystem::path& directory, FileEntryVec& entries, const std::vector<std::string>& patterns, bool recursive = false)`:
  Scans the directory for files matching the provided regex patterns. Supports recursive traversal.
- `void scan(const std::filesystem::path& directory, FileEntryVec& entries, Callable filter, bool recursive = false)`:
  Scans the directory using a custom filter (e.g., lambda functions) to include or exclude files.

### `FileEntryContainer`

The `FileEntryContainer` class manages collections of `FileEntry` objects.

**Key Methods**:
- `void append(const FileEntry& entry)`: Adds a `FileEntry` to the container.
- `void append(const std::filesystem::path& path)`: Adds a file path as a `FileEntry` to the container.
- `FileEntry& operator[](std::size_t index) const`: Accesses a file entry by index.
- `size_t size() const`: Returns the number of entries in the container.
- `bool foreach(const std::function<bool(const FileEntry&)>& callback) const`: Iterates over entries, executing the callback for each.

---

## Example Usage

### FileEntry

```cpp
#include "FileEntry.h"

int main() {
    FileEntry entry("/path/to/file.txt");

    if (entry.exists()) {
        std::cout << "File size: " << entry.getSize() << " bytes\n";
        std::cout << "File content:\n" << entry.getContent() << std::endl;
    }

    return 0;
}
```

### FileScanner

```cpp
#include "FileScanner.h"

int main() {
    FileScanner& scanner = FileScanner::getInstance();
    FileEntryVec entries;

    // Scan for all .txt files in a directory (recursively)
    std::vector<std::string> patterns = {R"(.*\.txt$)"};
    scanner.scan("/path/to/directory", entries, patterns, true);

    for (const auto& entry : entries) {
        std::cout << "Found file: " << entry->getPath() << std::endl;
    }

    return 0;
}
```

### FileEntryContainer

```cpp
#include "FileEntryContainer.h"

int main() {
    FileEntryContainer container;

    container.append("/path/to/file1.txt");
    container.append("/path/to/file2.log");

    container.foreach([](const FileEntry& entry) {
        std::cout << "File: " << entry.getPath() << "\n";
        return true; // Continue iteration
    });

    return 0;
}
```

---

## Requirements

- **C++17** or later
- **Standard Libraries**:
    - `<filesystem>`: For file and directory operations.
    - `<regex>`: For filename filtering.
    - `<functional>`: For custom predicates.

---

## Build Instructions

### Using CMake

1. Create a `CMakeLists.txt` file:

```cmake
cmake_minimum_required(VERSION 3.12)
project(fileentries)

set(CMAKE_CXX_STANDARD 17)

add_library(fileentries STATIC
    src/FileEntry.cpp
    src/FileScanner.cpp
    src/FileEntryContainer.cpp
)

target_include_directories(fileentries PUBLIC include)
```

2. Build the library:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

---

## Contributing

Contributions are welcome! Please submit a pull request or open an issue to report bugs or suggest features.

---

## License

This library is open-source and available under the MIT License. See the LICENSE file for details.

