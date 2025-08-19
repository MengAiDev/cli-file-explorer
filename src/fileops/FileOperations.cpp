#include "../../include/fileops/FileOperations.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <queue>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#endif

bool FileOperations::copyFile(const std::string& source, const std::string& destination) {
    if (!confirmOperation("Copy", source)) {
        return false;
    }

#ifdef _WIN32
    if (CopyFile(source.c_str(), destination.c_str(), FALSE)) {
        return true;
    }
#else
    std::ifstream src(source, std::ios::binary);
    std::ofstream dst(destination, std::ios::binary);
    
    if (src && dst) {
        dst << src.rdbuf();
        return true;
    }
#endif
    return false;
}

bool FileOperations::moveFile(const std::string& source, const std::string& destination) {
    if (!confirmOperation("Move", source)) {
        return false;
    }

#ifdef _WIN32
    if (MoveFile(source.c_str(), destination.c_str())) {
        return true;
    }
#else
    if (rename(source.c_str(), destination.c_str()) == 0) {
        return true;
    }
#endif
    return false;
}

bool FileOperations::deleteFile(const std::string& path) {
    if (!confirmOperation("Delete", path)) {
        return false;
    }

#ifdef _WIN32
    if (DeleteFile(path.c_str())) {
        return true;
    }
#else
    if (unlink(path.c_str()) == 0) {
        return true;
    }
#endif
    return false;
}

bool FileOperations::createDirectory(const std::string& path) {
    if (!confirmOperation("Create directory", path)) {
        return false;
    }

#ifdef _WIN32
    if (CreateDirectory(path.c_str(), NULL)) {
        return true;
    }
#else
    if (mkdir(path.c_str(), 0755) == 0) {
        return true;
    }
#endif
    return false;
}

bool FileOperations::createFile(const std::string& path) {
    if (!confirmOperation("Create file", path)) {
        return false;
    }

    // Create an empty file by opening it in output mode and immediately closing it
    std::ofstream file(path, std::ios::out);
    if (file.is_open()) {
        file.close();
        return true;
    }
    return false;
}

std::vector<std::string> FileOperations::searchFiles(const std::string& directory, const std::string& pattern) {
    std::vector<std::string> results;
    std::regex regexPattern(pattern);
    
    std::queue<std::string> directories;
    directories.push(directory);
    
    while (!directories.empty()) {
        std::string currentDir = directories.front();
        directories.pop();
        
#ifdef _WIN32
        WIN32_FIND_DATA findData;
        HANDLE hFind = FindFirstFile((currentDir + "\\*").c_str(), &findData);
        
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                std::string fileName(findData.cFileName);
                if (fileName != "." && fileName != "..") {
                    std::string fullPath = currentDir + "\\" + fileName;
                    
                    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        directories.push(fullPath);
                    } else {
                        if (std::regex_search(fileName, regexPattern)) {
                            results.push_back(fullPath);
                        }
                    }
                }
            } while (FindNextFile(hFind, &findData));
            FindClose(hFind);
        }
#else
        DIR* dir = opendir(currentDir.c_str());
        if (dir) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                std::string fileName(entry->d_name);
                if (fileName != "." && fileName != "..") {
                    std::string fullPath = currentDir + "/" + fileName;
                    
                    struct stat statBuf;
                    if (stat(fullPath.c_str(), &statBuf) == 0 && S_ISDIR(statBuf.st_mode)) {
                        directories.push(fullPath);
                    } else {
                        if (std::regex_search(fileName, regexPattern)) {
                            results.push_back(fullPath);
                        }
                    }
                }
            }
            closedir(dir);
        }
#endif
    }
    
    return results;
}

bool FileOperations::confirmOperation(const std::string& operation, const std::string& path) {
    std::cout << operation << " '" << path << "'? (y/N): ";
    std::string response;
    std::getline(std::cin, response);
    
    return (response == "y" || response == "Y");
}
