#define BOOST_TEST_MODULE FileScannerTest
#include <boost/test/included/unit_test.hpp>
#include "FileScanner.h"
#include <filesystem>
#include <fstream>
#include <algorithm>

// Function to sort FileEntryVec alphabetically by the full path name
void sortFileEntriesAlphabetically(FileEntryVec& entries) {
    std::sort(entries.begin(), entries.end(), [](const FileEntryPtr& a, const FileEntryPtr& b) {
        return a->getPath().string() < b->getPath().string();
    });
}

namespace fs = std::filesystem;

// Fixture for setting up and tearing down test resources
struct TestFixture {
    TestFixture() {
        testDataPath = fs::temp_directory_path() / "file_scanner_testdata";

        // Set up test directory structure
        fs::create_directories(testDataPath / "folder1");
        fs::create_directories(testDataPath / "folder2");

        // Create test files
        std::ofstream(testDataPath / "folder1/file1.txt") << "File 1 content";
        std::ofstream(testDataPath / "folder1/file2.log") << "File 2 content";
        std::ofstream(testDataPath / "folder2/subfile1.txt") << "Subfile 1 content";
        std::ofstream(testDataPath / "folder2/subfile2.log") << "Subfile 2 content";
    }

    ~TestFixture() {
        // Clean up the test directory structure
        fs::remove_all(testDataPath);
    }

    fs::path testDataPath;
};

BOOST_FIXTURE_TEST_SUITE(FileScannerSuite, TestFixture)

BOOST_FIXTURE_TEST_CASE(NonRecursiveScan, TestFixture) {
    FileScanner& scanner = FileScanner::getInstance();
    FileEntryVec entries;

    // Non-recursive scan in folder1
    scanner.scan(testDataPath / "folder1", entries, [](const fs::path&) { return true; }, false);

    // Sort entries alphabetically
    sortFileEntriesAlphabetically(entries);

    BOOST_TEST(entries.size() == 2);
    BOOST_TEST(entries[0]->getPath().filename().string() == "file1.txt");
    BOOST_TEST(entries[1]->getPath().filename().string() == "file2.log");
}

BOOST_FIXTURE_TEST_CASE(RecursiveScan, TestFixture) {
    FileScanner& scanner = FileScanner::getInstance();
    FileEntryVec entries;

    // Recursive scan in the root test data directory
    scanner.scan(testDataPath, entries, [](const fs::path&) { return true; }, true);

    // Sort entries alphabetically
    sortFileEntriesAlphabetically(entries);

    BOOST_TEST(entries.size() == 4);
    BOOST_TEST(entries[0]->getPath().filename().string() == "file1.txt");
    BOOST_TEST(entries[1]->getPath().filename().string() == "file2.log");
    BOOST_TEST(entries[2]->getPath().filename().string() == "subfile1.txt");
    BOOST_TEST(entries[3]->getPath().filename().string() == "subfile2.log");
}

BOOST_FIXTURE_TEST_CASE(ScanWithRegex, TestFixture) {
    FileScanner& scanner = FileScanner::getInstance();
    FileEntryVec entries;

    // Scan with regex to match only .txt files
    std::vector<std::string> patterns = {R"(.*\.txt$)"};
    scanner.scan(testDataPath, entries, patterns, true);

    // Sort entries alphabetically
    sortFileEntriesAlphabetically(entries);

    BOOST_TEST(entries.size() == 2);
    BOOST_TEST(entries[0]->getPath().filename().string() == "file1.txt");
    BOOST_TEST(entries[1]->getPath().filename().string() == "subfile1.txt");
}

BOOST_FIXTURE_TEST_CASE(ScanWithCustomFilter, TestFixture) {
    FileScanner& scanner = FileScanner::getInstance();
    FileEntryVec entries;

    // Scan with a custom filter to match only .log files
    scanner.scan(testDataPath, entries, [](const fs::path& path) {
        return path.extension() == ".log";
    }, true);

    // Sort entries alphabetically
    sortFileEntriesAlphabetically(entries);

    BOOST_TEST(entries.size() == 2);
    BOOST_TEST(entries[0]->getPath().filename().string() == "file2.log");
    BOOST_TEST(entries[1]->getPath().filename().string() == "subfile2.log");
}

BOOST_FIXTURE_TEST_CASE(ScanWithFileEntryContainer, TestFixture) {
    FileScanner& scanner = FileScanner::getInstance();
    FileEntryContainer entries;

    // Scan with a custom filter to match only .log files
    scanner.scan(testDataPath, entries, [](const fs::path& path) {
        return path.extension() == ".log";
    }, true);

    // Sort entries alphabetically
    entries.sortFileEntriesAlphabetically();

    BOOST_TEST(entries.size() == 2);
    BOOST_TEST(entries[0]->getPath().filename().string() == "file2.log");
    BOOST_TEST(entries[1]->getPath().filename().string() == "subfile2.log");
}

BOOST_AUTO_TEST_SUITE_END()
