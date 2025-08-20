#include "../../include/bookmark/BookMark.h"
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sstream>

// Static member definition
std::vector<std::string> BookMark::bookMarks;

bool BookMark::addBookMark(std::string path) {
    // Check if bookmark already exists
    if (std::find(bookMarks.begin(), bookMarks.end(), path) != bookMarks.end()) {
        return false; // Bookmark already exists
    }
    
    bookMarks.push_back(path);
    return true; // Successfully added
}

bool BookMark::removeBookMark(std::string path) {
    auto it = std::find(bookMarks.begin(), bookMarks.end(), path);
    if (it != bookMarks.end()) {
        bookMarks.erase(it);
        return true; // Successfully removed
    }
    return false; // Bookmark not found
}

bool BookMark::emptyBookMarks() {
    bool isEmpty = bookMarks.empty();
    bookMarks.clear();
    return !isEmpty; // Return true if there were elements to clear
}

bool BookMark::saveBookMarks() {
    // Get home directory path
    const char* homeDir = getenv("HOME");
    if (!homeDir) {
        // Fallback to passwd lookup if HOME is not set
        struct passwd* pw = getpwuid(getuid());
        if (!pw) {
            std::cerr << "Error: Could not determine home directory" << std::endl;
            return false;
        }
        homeDir = pw->pw_dir;
    }
    
    // Create full path to bookmarks file
    std::string bookmarksFile = std::string(homeDir) + "/.bookmarks.txt";
    
    // Open file for writing
    std::ofstream file(bookmarksFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open bookmarks file for writing: " << bookmarksFile << std::endl;
        return false;
    }
    
    // Write all bookmarks to file
    for (const auto& bookmark : bookMarks) {
        file << bookmark << std::endl;
    }
    
    file.close();
    return true;
}

bool BookMark::loadBookMarks() {
    // Get home directory path
    const char* homeDir = getenv("HOME");
    if (!homeDir) {
        // Fallback to passwd lookup if HOME is not set
        struct passwd* pw = getpwuid(getuid());
        if (!pw) {
            std::cerr << "Error: Could not determine home directory" << std::endl;
            return false;
        }
        homeDir = pw->pw_dir;
    }
    
    // Create full path to bookmarks file
    std::string bookmarksFile = std::string(homeDir) + "/.bookmarks.txt";
    
    // Open file for reading
    std::ifstream file(bookmarksFile);
    if (!file.is_open()) {
        // File doesn't exist, which is okay for first run
        return true;
    }
    
    // Clear existing bookmarks
    bookMarks.clear();
    
    // Read bookmarks from file
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            bookMarks.push_back(line);
        }
    }
    
    file.close();
    return true;
}