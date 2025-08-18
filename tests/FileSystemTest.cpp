#include "FileSystem.h"
#include <cassert>
#include <iostream>
#include <fstream>

void testListDirectory() {
    std::cout << "Testing listDirectory..." << std::endl;
    
    // Test listing current directory
    try {
        auto files = FileSystem::listDirectory("..");
        assert(!files.empty()); // Should at least contain this directory's files
        std::cout << "Found " << files.size() << " items in parent directory" << std::endl;
        
        // Print all file names for debugging
        std::cout << "Files found:" << std::endl;
        for (const auto& file : files) {
            std::cout << "  " << file.getName() << std::endl;
        }
        
        // Check that we have at least some expected files
        bool foundSrc = false;
        bool foundInclude = false;
        bool foundTests = false;
        bool foundDocs = false;
        bool foundCMakeLists = false;
        bool foundReadme = false;
        
        for (const auto& file : files) {
            if (file.getName() == "src") foundSrc = true;
            if (file.getName() == "include") foundInclude = true;
            if (file.getName() == "tests") foundTests = true;
            if (file.getName() == "docs") foundDocs = true;
            if (file.getName() == "CMakeLists.txt") foundCMakeLists = true;
            if (file.getName() == "README.md") foundReadme = true;
        }
        
        assert(foundSrc);
        assert(foundInclude);
        assert(foundTests);
        assert(foundDocs);
        assert(foundCMakeLists);
        assert(foundReadme);
        
        std::cout << "testListDirectory passed" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "testListDirectory failed: " << e.what() << std::endl;
        assert(false);
    }
}

void testChangeDirectory() {
    std::cout << "Testing changeDirectory..." << std::endl;
    
    std::string current_path = "..";
    
    // Test changing to src directory
    bool result = FileSystem::changeDirectory(current_path, "src");
    assert(result);
    assert(current_path.find("src") != std::string::npos);
    
    // Test changing back to parent
    result = FileSystem::changeDirectory(current_path, "..");
    assert(result);
    
    std::cout << "testChangeDirectory passed" << std::endl;
}

void testGetParentDirectory() {
    std::cout << "Testing getParentDirectory..." << std::endl;
    
    // Add debugging output
    std::string result = FileSystem::getParentDirectory("/");
    std::cout << "getParentDirectory(\"/\") returned: \"" << result << "\"" << std::endl;
    
#ifdef _WIN32
    assert(FileSystem::getParentDirectory("C:\\Users\\Test") == "C:\\Users");
    assert(FileSystem::getParentDirectory("C:\\") == "C:\\");
#else
    assert(FileSystem::getParentDirectory("/home/user") == "/home");
    assert(FileSystem::getParentDirectory("/") == "/");
#endif
    assert(FileSystem::getParentDirectory(".") == ".");
    
    std::cout << "testGetParentDirectory passed" << std::endl;
}

void testIsDirectory() {
    std::cout << "Testing isDirectory..." << std::endl;
    
    // Test with absolute paths since we're running from the build directory
    std::string current_dir = "..";
    std::string src_path = "../src";
    std::string include_path = "../include";
    
    assert(FileSystem::isDirectory(current_dir));
    assert(FileSystem::isDirectory(src_path));
    assert(FileSystem::isDirectory(include_path));
    assert(!FileSystem::isDirectory("nonexistent_file"));
    
    std::cout << "testIsDirectory passed" << std::endl;
}

void testFileExists() {
    std::cout << "Testing fileExists..." << std::endl;
    
    // Test with absolute paths since we're running from the build directory
    std::string cmake_lists_path = "../CMakeLists.txt";
    
    assert(FileSystem::fileExists(cmake_lists_path));
    assert(!FileSystem::fileExists("nonexistent_file"));
    
    std::cout << "testFileExists passed" << std::endl;
}

int main() {
    std::cout << "Running FileSystem tests..." << std::endl;
    
    testListDirectory();
    testChangeDirectory();
    testGetParentDirectory();
    testIsDirectory();
    testFileExists();
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
