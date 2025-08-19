#include "../../include/fileops/FileOperations.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <queue>
#include <iomanip>
#include <ctime>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#endif

bool FileOperations::createFile(const std::string& path) {
    // Create an empty file by opening it in output mode and immediately closing it
    std::ofstream file(path, std::ios::out);
    if (file.is_open()) {
        file.close();
        return true;
    }
    return false;
}

bool FileOperations::deleteFile(const std::string& path) {
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

// Format file size for human-readable display
std::string FileOperations::formatFileSize(size_t size) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double sizeDouble = static_cast<double>(size);
    
    while (sizeDouble >= 1024.0 && unitIndex < 4) {
        sizeDouble /= 1024.0;
        unitIndex++;
    }
    
    std::ostringstream oss;
    if (unitIndex == 0) {
        oss << size << " " << units[unitIndex];
    } else {
        oss << std::fixed << std::setprecision(1) << sizeDouble << " " << units[unitIndex];
    }
    
    return oss.str();
}

// Format time for display
std::string FileOperations::formatTime(const std::chrono::system_clock::time_point& tp) {
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    std::tm* tm = std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Format permissions for display
std::string FileOperations::formatPermissions(uint32_t permissions) {
    std::ostringstream oss;
    
#ifdef _WIN32
    // Simplified Windows permissions display
    if (permissions & 0444) {
        oss << "r--";
    } else {
        oss << "---";
    }
    
    if (permissions & 0222) {
        oss << "w--";
    } else {
        oss << "---";
    }
    
    if (permissions & 0111) {
        oss << "x--";
    } else {
        oss << "---";
    }
#else
    // Unix permissions display
    oss << ((permissions & S_IRUSR) ? 'r' : '-');
    oss << ((permissions & S_IWUSR) ? 'w' : '-');
    oss << ((permissions & S_IXUSR) ? 'x' : '-');
    oss << ((permissions & S_IRGRP) ? 'r' : '-');
    oss << ((permissions & S_IWGRP) ? 'w' : '-');
    oss << ((permissions & S_IXGRP) ? 'x' : '-');
    oss << ((permissions & S_IROTH) ? 'r' : '-');
    oss << ((permissions & S_IWOTH) ? 'w' : '-');
    oss << ((permissions & S_IXOTH) ? 'x' : '-');
#endif
    
    return oss.str();
}