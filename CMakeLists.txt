cmake_minimum_required(VERSION 3.10)
project(cli_file_explorer VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Add source files
set(SOURCES
    src/main.cpp
    src/FileSystem.cpp
    src/cli/CommandLineInterface.cpp
    src/cli/CommandLineInterface_interactive.cpp
    src/cli/CommandLineInterface_delete_rename.cpp
    src/cli/CommandLineInterface_copy.cpp
    src/fileops/FileOperations.cpp
    src/search/Search.cpp
    src/sorting/Sorting.cpp
    src/bookmark/BookMark.cpp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Find and link ncurses library
find_package(Curses REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_NCURSES)

# Link libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} shell32 shlwapi)
else()
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()

# Install targets
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Enable testing
enable_testing()

# Create test executable
add_executable(FileSystemTest tests/FileSystemTest.cpp src/FileSystem.cpp)
target_include_directories(FileSystemTest PRIVATE include)
target_compile_definitions(FileSystemTest PRIVATE USE_NCURSES)

# Add test
add_test(NAME FileSystemTest COMMAND FileSystemTest)

# Create bookmark test executable
add_executable(BookMarkTest tests/BookMarkTest.cpp src/bookmark/BookMark.cpp)
target_include_directories(BookMarkTest PRIVATE include)

# Add bookmark test
add_test(NAME BookMarkTest COMMAND BookMarkTest)
