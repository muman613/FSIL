#pragma once

#include <filesystem>
#include <fstream>

// Utility to create a temporary file for testing
class TempFile {

public:
    explicit TempFile(const std::string& content = "") : tempPath(std::filesystem::temp_directory_path() / "test_file.txt") {
        std::ofstream ofs(tempPath);
        if (ofs.is_open()) {
            ofs << content;
            ofs.close();
        }
    }

    ~TempFile() {
        std::filesystem::remove(tempPath);
    }

    [[nodiscard]] std::filesystem::path getPath() const {
        return tempPath;
    }

private:
    std::filesystem::path tempPath;
};

