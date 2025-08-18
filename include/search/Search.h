#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>
#include <regex>
#include <filesystem>

class Search {
public:
    static std::vector<std::string> searchByName(const std::string& directory, const std::string& pattern, bool recursive = true);
    static std::vector<std::string> searchByContent(const std::string& directory, const std::string& pattern, bool recursive = true);
    
private:
    static void searchByNameRecursive(const std::string& directory, const std::regex& pattern, std::vector<std::string>& results);
    static void searchByContentRecursive(const std::string& directory, const std::string& pattern, std::vector<std::string>& results);
};

#endif // SEARCH_H
