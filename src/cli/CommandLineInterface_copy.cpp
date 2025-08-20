#include "../../include/cli/CommandLineInterface.h"
#include "../../include/fileops/FileOperations.h"
#include <iostream>
#include <dirent.h>
#include <cstring>

void CommandLineInterface::copyFile(const std::string& sourcePath, const std::string& destinationPath) {
    std::string fullSourcePath = currentPath;
    std::string fullDestinationPath = currentPath;
    
    // Add appropriate path separator
#ifdef _WIN32
    if (fullSourcePath.back() != '\\' && fullSourcePath.back() != '/') {
        fullSourcePath += "\\";
        fullDestinationPath += "\\";
    }
#else
    if (fullSourcePath.back() != '/') {
        fullSourcePath += "/";
        fullDestinationPath += "/";
    }
#endif
    
    fullSourcePath += sourcePath;
    fullDestinationPath += destinationPath;
    
    // Check if source exists
    if (!FileOperations::exists(fullSourcePath)) {
        std::cout << "Source file or directory does not exist: " << sourcePath << "\n";
        return;
    }
    
    // Check if destination already exists
    if (FileOperations::exists(fullDestinationPath)) {
        std::cout << "Destination already exists: " << destinationPath << "\n";
        return;
    }
    
    if (FileOperations::copyFile(fullSourcePath, fullDestinationPath)) {
        std::cout << "Copied successfully: " << sourcePath << " -> " << destinationPath << "\n";
    } else {
        std::cout << "Failed to copy: " << sourcePath << " -> " << destinationPath << "\n";
    }
}