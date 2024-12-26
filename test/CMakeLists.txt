project(FileEntryTest)

# Add Boost Test dependency
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

function(add_unit_test TESTNAME)
    # The remaining arguments are treated as SRCS
    set(SRCS ${ARGN})
    message(STATUS "Adding unit test ${TESTNAME}")
    add_executable(${TESTNAME} ${SRCS})
    target_link_libraries(${TESTNAME} PRIVATE fileentries Boost::unit_test_framework)
endfunction()

# Add unit tests here

add_unit_test(FileEntryTest FileEntryTest.cpp TempFile.h)
add_unit_test(FileScannerTest FileScannerTest.cpp)
add_unit_test(FileEntryContainerTest FileEntryContainerTest.cpp)
