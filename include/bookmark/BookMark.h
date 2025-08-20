#ifndef BOOKMARK_H
#define BOOKMARK_H 

#include <vector>
#include <string>

class BookMark {
    public:
        static std::vector<std::string> bookMarks;
        static bool addBookMark(std::string path);
        static bool removeBookMark(std::string path);
        static bool emptyBookMarks();
        static bool saveBookMarks();
        static bool loadBookMarks();
};

#endif