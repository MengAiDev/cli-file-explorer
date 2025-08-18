#include "../../include/search/Search.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <filesystem>

std::vector<std::string> Search::searchByName(const std::string& directory, const std::string& pattern, bool recursive) {
    std::vector<std::string> results;
    std::regex regexPattern(pattern);
    
    if (recursive) {
        searchByNameRecursive(directory, regexPattern, results);
    } else {
        // Non-recursive search in single directory
        try {
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                std::string filename = entry.path().filename().string();
                if (std::regex_search(filename, regexPattern)) {
                    results.push_back(entry.path().string());
                }
            }
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Error accessing directory: " << ex.what() << std::endl;
        }
    }
    
    return results;
}

std::vector<std::string> Search::searchByContent(const std::string& directory, const std::string& pattern, bool recursive) {
    std::vector<std::string> results;
    
    if (recursive) {
        searchByContentRecursive(directory, pattern, results);
    } else {
        // Non-recursive search in single directory
        try {
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    std::ifstream file(entry.path());
                    std::string line;
                    bool found = false;
                    
                    while (std::getline(file, line) && !found) {
                        if (line.find(pattern) != std::string::npos) {
                            results.push_back(entry.path().string());
                            found = true;
                        }
                    }
                }
            }
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Error accessing directory: " << ex.what() << std::endl;
        }
    }
    
    return results;
}

void Search::searchByNameRecursive(const std::string& directory, const std::regex& pattern, std::vector<std::string>& results) {
    std::queue<std::string> directories;
    directories.push(directory);
    
    while (!directories.empty()) {
        std::string currentDir = directories.front();
        directories.pop();
        
        try {
            for (const auto& entry : std::filesystem::directory_iterator(currentDir)) {
                std::string filename = entry.path().filename().string();
                
                if (std::regex_search(filename, pattern)) {
                    results.push_back(entry.path().string());
                }
                
                if (entry.is_directory()) {
                    directories.push(entry.path().string());
                }
            }
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Error accessing directory: " << ex.what() << std::endl;
        }
    }
}

void Search::searchByContentRecursive(const std::string& directory, const std::string& pattern, std::vector<std::string>& results) {
    std::queue<std::string> directories;
    directories.push(directory);
    
    while (!directories.empty()) {
        std::string currentDir = directories.front();
        directories.pop();
        
        try {
            for (const auto& entry : std::filesystem::directory_iterator(currentDir)) {
                if (entry.is_regular_file()) {
                    std::ifstream file(entry.path());
                    std::string line;
                    bool found = false;
                    
                    while (std::getline(file, line) && !found) {
                        if (line.find(pattern) != std::string::npos) {
                            results.push_back(entry.path().string());
                            found = true;
                        }
                    }
                } else if (entry.is_directory()) {
                    directories.push(entry.path().string());
                }
            }
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Error accessing directory: " << ex.what() << std::endl;
        }
    }
}
