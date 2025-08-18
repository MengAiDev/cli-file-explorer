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
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Install targets
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Enable testing
enable_testing()

# Create test executable
add_executable(FileSystemTest tests/FileSystemTest.cpp src/FileSystem.cpp)
target_include_directories(FileSystemTest PRIVATE include)

# Add test
add_test(NAME FileSystemTest COMMAND FileSystemTest)
