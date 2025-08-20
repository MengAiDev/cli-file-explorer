#ifndef FILEOPS_FILEOPERATIONS_H
#define FILEOPS_FILEOPERATIONS_H

#include <string>
#include <chrono>

class FileOperations {
public:
    static bool createFile(const std::string& filePath);
    static bool deleteFile(const std::string& filePath);
    static bool deleteDirectory(const std::string& dirPath);
    static bool renameFile(const std::string& oldPath, const std::string& newPath);
    static bool copyFile(const std::string& sourcePath, const std::string& destinationPath);
    static bool exists(const std::string& path);
    
    // Add helper functions for formatting
    static std::string formatFileSize(size_t size);
    static std::string formatTime(const std::chrono::system_clock::time_point& tp);
    static std::string formatPermissions(uint32_t permissions);
};

#endif // FILEOPS_FILEOPERATIONS_H