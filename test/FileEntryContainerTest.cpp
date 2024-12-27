#define BOOST_TEST_MODULE FileEntryContainerTest
#include <boost/test/included/unit_test.hpp>
#include "FileEntryContainer.h"

namespace fs = std::filesystem;

// Fixture for setting up test environment
struct TestFixture {
    TestFixture() {
        testFilePaths = {
            fs::temp_directory_path() / "test_file1.txt",
            fs::temp_directory_path() / "test_file2.log",
            fs::temp_directory_path() / "test_file3.txt"
        };

        // Create test files
        for (const auto& path : testFilePaths) {
            std::ofstream ofs(path);
            ofs << "Test content";
        }
    }

    ~TestFixture() {
        // Clean up test files
        for (const auto& path : testFilePaths) {
            fs::remove(path);
        }
    }

    std::vector<fs::path> testFilePaths;
};

BOOST_FIXTURE_TEST_SUITE(FileEntryContainerSuite, TestFixture)

BOOST_AUTO_TEST_CASE(ConstructorWithInitializerList) {
    // Create a FileEntryContainer with an initializer list
    FileEntryContainer container({
        FileEntry(testFilePaths[0]),
        FileEntry(testFilePaths[1]),
        FileEntry(testFilePaths[2])
    });

    // Verify size
    BOOST_TEST(container.size() == 3);

    // Verify paths
    BOOST_TEST(container[0]->getPath() == testFilePaths[0]);
    BOOST_TEST(container[1]->getPath() == testFilePaths[1]);
    BOOST_TEST(container[2]->getPath() == testFilePaths[2]);
}

BOOST_AUTO_TEST_CASE(AppendEntry) {
    FileEntryContainer container;

    // Append entries
    container.append(FileEntry(testFilePaths[0]));
    container.append(FileEntry(testFilePaths[1]));

    // Verify size
    BOOST_TEST(container.size() == 2);

    // Verify paths
    BOOST_TEST(container[0]->getPath() == testFilePaths[0]);
    BOOST_TEST(container[1]->getPath() == testFilePaths[1]);
}

BOOST_AUTO_TEST_CASE(AppendPath) {
    FileEntryContainer container;

    // Append paths
    container.append(testFilePaths[0]);
    container.append(testFilePaths[2]);

    // Verify size
    BOOST_TEST(container.size() == 2);

    // Verify paths
    BOOST_TEST(container[0]->getPath() == testFilePaths[0]);
    BOOST_TEST(container[1]->getPath() == testFilePaths[2]);
}

BOOST_AUTO_TEST_CASE(OperatorAccess) {
    FileEntryContainer container({
        FileEntry(testFilePaths[0]),
        FileEntry(testFilePaths[1])
    });

    // Verify operator[] functionality
    BOOST_TEST(container[0]->getPath() == testFilePaths[0]);
    BOOST_TEST(container[1]->getPath() == testFilePaths[1]);
}

BOOST_AUTO_TEST_CASE(ForEach) {
    FileEntryContainer container({
        FileEntry(testFilePaths[0]),
        FileEntry(testFilePaths[1]),
        FileEntry(testFilePaths[2])
    });

    size_t count = 0;

    // Use foreach to count entries
    container.foreach([&count](const FileEntry& entry) {
        BOOST_TEST(entry.exists());
        BOOST_TEST(fs::exists(entry.getPath()));
        ++count;
        return true;
    });

    BOOST_TEST(count == container.size());
}

BOOST_AUTO_TEST_CASE(MoveConstructor) {
    FileEntryContainer original({
        FileEntry(testFilePaths[0]),
        FileEntry(testFilePaths[1])
    });

    // Use move constructor
    FileEntryContainer moved(std::move(original));

    // Verify moved container
    BOOST_TEST(moved.size() == 2);
    BOOST_TEST(moved[0]->getPath() == testFilePaths[0]);
    BOOST_TEST(moved[1]->getPath() == testFilePaths[1]);
    BOOST_TEST(original.size() == 0);
}

//  TODO: Do this

BOOST_AUTO_TEST_SUITE_END()
