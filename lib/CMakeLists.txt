project(fileentries)


add_library(${PROJECT_NAME} STATIC FileEntry.cpp FileEntry.h
        FileScanner.cpp
        FileScanner.h
        FileScanner.tpp
        FileEntryContainer.cpp
        FileEntryContainer.h)
target_include_directories(${PROJECT_NAME} PUBLIC .)