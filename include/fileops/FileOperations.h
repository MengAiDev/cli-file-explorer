#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <string>
#include <vector>
#include <filesystem>

class FileOperations {
public:
    static bool copyFile(const std::string& source, const std::string& destination);
    static bool moveFile(const std::string& source, const std::string& destination);
    static bool deleteFile(const std::string& path);
    static bool createDirectory(const std::string& path);
    static bool createFile(const std::string& path);
    static std::vector<std::string> searchFiles(const std::string& directory, const std::string& pattern);

private:
    static bool confirmOperation(const std::string& operation, const std::string& path);
};

#endif // FILEOPERATIONS_H
