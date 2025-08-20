#include "../include/bookmark/BookMark.h"
#include <iostream>
#include <cassert>
#include <unistd.h>
#include <fstream>
#include <pwd.h>

void testAddBookMark() {
    // Clear any existing bookmarks
    BookMark::emptyBookMarks();
    
    // Test adding a bookmark
    bool result = BookMark::addBookMark("/home/user/test");
    assert(result == true);
    
    // Test adding duplicate bookmark
    result = BookMark::addBookMark("/home/user/test");
    assert(result == false);
    
    std::cout << "testAddBookMark passed\n";
}

void testRemoveBookMark() {
    // Clear any existing bookmarks
    BookMark::emptyBookMarks();
    
    // Add a bookmark
    BookMark::addBookMark("/home/user/test");
    
    // Test removing existing bookmark
    bool result = BookMark::removeBookMark("/home/user/test");
    assert(result == true);
    
    // Test removing non-existing bookmark
    result = BookMark::removeBookMark("/home/user/nonexistent");
    assert(result == false);
    
    std::cout << "testRemoveBookMark passed\n";
}

void testEmptyBookMarks() {
    // Clear any existing bookmarks
    BookMark::emptyBookMarks();
    
    // Add some bookmarks
    BookMark::addBookMark("/home/user/test1");
    BookMark::addBookMark("/home/user/test2");
    
    // Test clearing bookmarks
    bool result = BookMark::emptyBookMarks();
    assert(result == true);
    
    // Test clearing when already empty
    result = BookMark::emptyBookMarks();
    assert(result == false);
    
    std::cout << "testEmptyBookMarks passed\n";
}

void testSaveAndLoadBookMarks() {
    // Clear any existing bookmarks
    BookMark::emptyBookMarks();
    
    // Add some bookmarks
    BookMark::addBookMark("/home/user/test1");
    BookMark::addBookMark("/home/user/test2");
    
    // Save bookmarks
    bool saveResult = BookMark::saveBookMarks();
    assert(saveResult == true);
    
    // Clear bookmarks in memory
    BookMark::emptyBookMarks();
    
    // Load bookmarks
    bool loadResult = BookMark::loadBookMarks();
    assert(loadResult == true);
    
    // Check that bookmarks were loaded correctly
    assert(BookMark::bookMarks.size() == 2);
    assert(BookMark::bookMarks[0] == "/home/user/test1");
    assert(BookMark::bookMarks[1] == "/home/user/test2");
    
    // Clean up the test file
    const char* homeDir = getenv("HOME");
    if (!homeDir) {
        struct passwd* pw = getpwuid(getuid());
        homeDir = pw->pw_dir;
    }
    std::string bookmarksFile = std::string(homeDir) + "/.bookmarks.txt";
    unlink(bookmarksFile.c_str());
    
    std::cout << "testSaveAndLoadBookMarks passed\n";
}

int main() {
    testAddBookMark();
    testRemoveBookMark();
    testEmptyBookMarks();
    testSaveAndLoadBookMarks();
    
    std::cout << "All tests passed!\n";
    return 0;
}