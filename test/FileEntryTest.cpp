#define BOOST_TEST_MODULE FileEntryTest
#include <boost/test/included/unit_test.hpp>
#include "FileEntry.h"
#include <filesystem>
#include <fstream>
#include <string>
#include "TempFile.h"

namespace fs = std::filesystem;

BOOST_AUTO_TEST_SUITE(FileEntrySuite)

BOOST_AUTO_TEST_CASE(ConstructorInitialization) {
    TempFile tempFile("This is a test file.");
    FileEntry fileEntry(tempFile.getPath());

    BOOST_CHECK(fileEntry.getPath() == tempFile.getPath());
}

BOOST_AUTO_TEST_CASE(FileExists) {
    TempFile tempFile("This is a test file.");
    FileEntry fileEntry(tempFile.getPath());

    BOOST_CHECK(fileEntry.exists());
}

BOOST_AUTO_TEST_CASE(FileSize) {
    TempFile tempFile("This is a test file.");
    FileEntry fileEntry(tempFile.getPath());

    BOOST_CHECK(fileEntry.getSize() == 20); // 21 bytes: "This is a test file."
}

BOOST_AUTO_TEST_CASE(FileTimeRetrieval) {
    TempFile tempFile("This is a test file.");
    FileEntry fileEntry(tempFile.getPath());

    auto time = fileEntry.getModificationTime(); // Capture the return value to respect [[nodiscard]]
    BOOST_CHECK_NO_THROW(time);
}

BOOST_AUTO_TEST_CASE(FileNameRetrieval) {
    TempFile tempFile("This is a test file.");
    FileEntry fileEntry(tempFile.getPath());

    BOOST_CHECK(fileEntry.getName() == "test_file.txt");
}

BOOST_AUTO_TEST_CASE(FileExtensionRetrieval) {
    TempFile tempFile("This is a test file.");
    FileEntry fileEntry(tempFile.getPath());

    BOOST_CHECK(fileEntry.getExtension() == ".txt");
}

BOOST_AUTO_TEST_CASE(FileContentRead) {
    TempFile tempFile("This is a test file.");
    FileEntry fileEntry(tempFile.getPath());

    BOOST_CHECK(fileEntry.getContent() == "This is a test file.");
}

BOOST_AUTO_TEST_CASE(PathUpdate) {
    TempFile tempFile1("This is a test file.");
    TempFile tempFile2("Another test file.");
    FileEntry fileEntry(tempFile1.getPath());

    fileEntry.setPath(tempFile2.getPath());
    BOOST_CHECK(fileEntry.getPath() == tempFile2.getPath());
    BOOST_CHECK(fileEntry.getContent() == "Another test file.");
}

BOOST_AUTO_TEST_CASE(FileGetLines) {
    // Create a temporary file with multiple lines
    TempFile tempFile("Line 1\nLine 2\nLine 3\nLine 4");

    // Initialize the FileEntry object with the temporary file's path
    FileEntry fileEntry(tempFile.getPath());

    // Call getLines() to retrieve the lines from the file
    std::vector<std::string> lines = fileEntry.getLines();

    // Check that the lines vector contains the expected number of lines
    BOOST_CHECK_EQUAL(lines.size(), 4);

    // Check the content of each line
    BOOST_CHECK(lines[0] == "Line 1");
    BOOST_CHECK(lines[1] == "Line 2");
    BOOST_CHECK(lines[2] == "Line 3");
    BOOST_CHECK(lines[3] == "Line 4");
}

BOOST_AUTO_TEST_CASE(FileEntryCreation) {
    TempFile tempFile("This is a test file.");
    TempFile tempFile2("Another test file.");

    FileEntryVec fileVec;

    fileVec.emplace_back(FileEntry::newEntry(tempFile.getPath()));
    fileVec.emplace_back(FileEntry::newEntry(tempFile2.getPath()));

    BOOST_CHECK_EQUAL(fileVec.size(), 2);
    BOOST_CHECK(fileVec[0]->getPath() == tempFile.getPath());
    BOOST_CHECK(fileVec[1]->getPath() == tempFile2.getPath());
}

BOOST_AUTO_TEST_SUITE_END()
