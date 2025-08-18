#include "../../include/sorting/Sorting.h"

void Sorting::sortFiles(std::vector<FileInfo>& files, SortCriteria criteria, SortOrder order) {
    switch (criteria) {
        case SortCriteria::NAME:
            std::sort(files.begin(), files.end(), 
                      [order](const FileInfo& a, const FileInfo& b) {
                          return order == SortOrder::ASCENDING ? 
                                 compareByName(a, b) : compareByName(b, a);
                      });
            break;
        case SortCriteria::SIZE:
            std::sort(files.begin(), files.end(), 
                      [order](const FileInfo& a, const FileInfo& b) {
                          return order == SortOrder::ASCENDING ? 
                                 compareBySize(a, b) : compareBySize(b, a);
                      });
            break;
        case SortCriteria::DATE:
            std::sort(files.begin(), files.end(), 
                      [order](const FileInfo& a, const FileInfo& b) {
                          return order == SortOrder::ASCENDING ? 
                                 compareByDate(a, b) : compareByDate(b, a);
                      });
            break;
        case SortCriteria::TYPE:
            std::sort(files.begin(), files.end(), 
                      [order](const FileInfo& a, const FileInfo& b) {
                          return order == SortOrder::ASCENDING ? 
                                 compareByType(a, b) : compareByType(b, a);
                      });
            break;
    }
}

bool Sorting::compareByName(const FileInfo& a, const FileInfo& b) {
    return a.getName() < b.getName();
}

bool Sorting::compareBySize(const FileInfo& a, const FileInfo& b) {
    return a.getSize() < b.getSize();
}

bool Sorting::compareByDate(const FileInfo& a, const FileInfo& b) {
    return a.getModifiedTime() < b.getModifiedTime();
}

bool Sorting::compareByType(const FileInfo& a, const FileInfo& b) {
    // Sort by type first (directory < file < symlink < unknown)
    if (a.getType() != b.getType()) {
        return static_cast<int>(a.getType()) < static_cast<int>(b.getType());
    }
    // If same type, sort by name
    return a.getName() < b.getName();
}
