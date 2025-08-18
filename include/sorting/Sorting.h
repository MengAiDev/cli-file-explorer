#ifndef SORTING_H
#define SORTING_H

#include "../include/FileSystem.h"
#include <vector>
#include <algorithm>

class Sorting {
public:
    enum class SortCriteria {
        NAME,
        SIZE,
        DATE,
        TYPE
    };
    
    enum class SortOrder {
        ASCENDING,
        DESCENDING
    };
    
    static void sortFiles(std::vector<FileInfo>& files, SortCriteria criteria, SortOrder order = SortOrder::ASCENDING);
    
private:
    static bool compareByName(const FileInfo& a, const FileInfo& b);
    static bool compareBySize(const FileInfo& a, const FileInfo& b);
    static bool compareByDate(const FileInfo& a, const FileInfo& b);
    static bool compareByType(const FileInfo& a, const FileInfo& b);
};

#endif // SORTING_H
