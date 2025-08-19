#include "../../include/cli/CommandLineInterface.h"
#include "../../include/fileops/FileOperations.h"
#include <iostream>
#include <dirent.h>
#include <cstring>

void CommandLineInterface::deleteFile(const std::string& path) {
    std::string fullPath = currentPath;
    
    // Add appropriate path separator
#ifdef _WIN32
    if (fullPath.back() != '\\' && fullPath.back() != '/') {
        fullPath += "\\";
    }
#else
    if (fullPath.back() != '/') {
        fullPath += "/";
    }
#endif
    
    fullPath += path;
    
    // Check if path exists
    if (!FileOperations::exists(fullPath)) {
        std::cout << "File or directory does not exist: " << path << "\n";
        return;
    }
    
    // Check if it's a directory
    DIR* dir = opendir(fullPath.c_str());
    if (dir) {
        closedir(dir);
        std::cout << "Use 'rmdir' to delete directories: " << path << "\n";
        return;
    }
    
    if (FileOperations::deleteFile(fullPath)) {
        std::cout << "File deleted successfully: " << path << "\n";
    } else {
        std::cout << "Failed to delete file: " << path << "\n";
    }
}

void CommandLineInterface::deleteDirectory(const std::string& path) {
    std::string fullPath = currentPath;
    
    // Add appropriate path separator
#ifdef _WIN32
    if (fullPath.back() != '\\' && fullPath.back() != '/') {
        fullPath += "\\";
    }
#else
    if (fullPath.back() != '/') {
        fullPath += "/";
    }
#endif
    
    fullPath += path;
    
    // Check if path exists
    if (!FileOperations::exists(fullPath)) {
        std::cout << "Directory does not exist: " << path << "\n";
        return;
    }
    
    // Check if it's a directory
    DIR* dir = opendir(fullPath.c_str());
    if (!dir) {
        std::cout << "Not a directory: " << path << "\n";
        return;
    }
    
    // Check if directory is empty
    struct dirent* entry;
    bool isEmpty = true;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            isEmpty = false;
            break;
        }
    }
    closedir(dir);
    
    if (!isEmpty) {
        std::cout << "Directory is not empty: " << path << "\n";
        std::cout << "Please remove all contents first or use shell command: rm -r " << path << "\n";
        return;
    }
    
    if (FileOperations::deleteDirectory(fullPath)) {
        std::cout << "Directory deleted successfully: " << path << "\n";
    } else {
        std::cout << "Failed to delete directory: " << path << "\n";
    }
}

void CommandLineInterface::renameFile(const std::string& oldPath, const std::string& newPath) {
    std::string fullOldPath = currentPath;
    std::string fullNewPath = currentPath;
    
    // Add appropriate path separator
#ifdef _WIN32
    if (fullOldPath.back() != '\\' && fullOldPath.back() != '/') {
        fullOldPath += "\\";
        fullNewPath += "\\";
    }
#else
    if (fullOldPath.back() != '/') {
        fullOldPath += "/";
        fullNewPath += "/";
    }
#endif
    
    fullOldPath += oldPath;
    fullNewPath += newPath;
    
    // Check if source exists
    if (!FileOperations::exists(fullOldPath)) {
        std::cout << "Source file or directory does not exist: " << oldPath << "\n";
        return;
    }
    
    // Check if destination already exists
    if (FileOperations::exists(fullNewPath)) {
        std::cout << "Destination already exists: " << newPath << "\n";
        return;
    }
    
    if (FileOperations::renameFile(fullOldPath, fullNewPath)) {
        std::cout << "Renamed successfully: " << oldPath << " -> " << newPath << "\n";
    } else {
        std::cout << "Failed to rename: " << oldPath << " -> " << newPath << "\n";
    }
}