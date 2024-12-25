project(FileEntryTest)

# Add Boost Test dependency
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
# Add the unit test executable
add_executable(FileEntryTest FileEntryTest.cpp TempFile.h)

# Link the unit test executable with the fileentries library and Boost Test
target_link_libraries(FileEntryTest
        PRIVATE
        fileentries
        Boost::unit_test_framework
)

add_executable(FileScannerTest FileScannerTest.cpp)

# Link the unit test executable with the fileentries library and Boost Test
target_link_libraries(FileScannerTest
        PRIVATE
        fileentries
        Boost::unit_test_framework
)

add_executable(FileEntryContainerTest FileEntryContainerTest.cpp)

# Link the unit test executable with the fileentries library and Boost Test
target_link_libraries(FileEntryContainerTest
        PRIVATE
        fileentries
        Boost::unit_test_framework
)
