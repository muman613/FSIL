cmake_minimum_required(VERSION 3.29)
project(fileEntry)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)

#add_executable(fileEntry main.cpp)
#
#target_link_libraries(fileEntry fileentries)

add_subdirectory(test)
