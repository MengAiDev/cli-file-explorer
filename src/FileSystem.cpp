#include "FileSystem.h"
#include <stdexcept>
#include <algorithm>
#include <filesystem>
#include <iostream>

#ifdef _WIN32
#include <direct.h>
#define stat _stat
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG)
#else
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

// FileInfo implementation
FileInfo::FileInfo(const std::string& name, FileType type, size_t size,
                   std::chrono::system_clock::time_point modified_time,
                   uint32_t permissions)
    : name_(name), type_(type), size_(size), modified_time_(modified_time), permissions_(permissions) {}

const std::string& FileInfo::getName() const {
    return name_;
}

FileInfo::FileType FileInfo::getType() const {
    return type_;
}

size_t FileInfo::getSize() const {
    return size_;
}

std::chrono::system_clock::time_point FileInfo::getModifiedTime() const {
    return modified_time_;
}

uint32_t FileInfo::getPermissions() const {
    return permissions_;
}

// Helper function to convert time_t to system_clock::time_point
std::chrono::system_clock::time_point time_t_to_time_point(time_t t) {
    return std::chrono::system_clock::from_time_t(t);
}

// Platform-specific FileInfo creation
FileInfo FileSystem::createFileInfo(const std::string& name,
#ifdef _WIN32
                                    WIN32_FIND_DATA& find_data
#else
                                    struct stat& stat_buf
#endif
                                   ) {
#ifdef _WIN32
    // Windows implementation
    FileInfo::FileType type;
    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        type = FileInfo::FileType::DIRECTORY;
    } else if (find_data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
        type = FileInfo::FileType::SYMLINK;
    } else {
        type = FileInfo::FileType::FILE;
    }

    // File size
    size_t size = (static_cast<size_t>(find_data.nFileSizeHigh) << 32) | find_data.nFileSizeLow;

    // Modified time
    FILETIME ft = find_data.ftLastWriteTime;
    ULARGE_INTEGER ull;
    ull.LowPart = ft.dwLowDateTime;
    ull.HighPart = ft.dwHighDateTime;
    time_t t = (ull.QuadPart / 10000000ULL - 11644473600ULL);
    auto modified_time = time_t_to_time_point(t);

    // Permissions (simplified)
    uint32_t permissions = 0;
    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
        permissions = 0444; // Read-only
    } else {
        permissions = 0666; // Read-write
    }

    return FileInfo(name, type, size, modified_time, permissions);
#else
    // Unix implementation
    FileInfo::FileType type;
    if (S_ISDIR(stat_buf.st_mode)) {
        type = FileInfo::FileType::DIRECTORY;
    } else if (S_ISLNK(stat_buf.st_mode)) {
        type = FileInfo::FileType::SYMLINK;
    } else if (S_ISREG(stat_buf.st_mode)) {
        type = FileInfo::FileType::FILE;
    } else {
        type = FileInfo::FileType::UNKNOWN;
    }

    size_t size = stat_buf.st_size;
    auto modified_time = time_t_to_time_point(stat_buf.st_mtime);
    uint32_t permissions = stat_buf.st_mode & 0777;

    return FileInfo(name, type, size, modified_time, permissions);
#endif
}

// List directory contents
std::vector<FileInfo> FileSystem::listDirectory(const std::string& path) {
    std::vector<FileInfo> files;
    std::string search_path = path;

#ifdef _WIN32
    // Windows implementation
    if (search_path.empty()) {
        search_path = ".";
    }
    if (search_path.back() != '\\' && search_path.back() != '/') {
        search_path += "\\";
    }
    search_path += "*";

    WIN32_FIND_DATA find_data;
    HANDLE handle = FindFirstFile(search_path.c_str(), &find_data);

    if (handle == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to list directory: " + path);
    }

    do {
        std::string name(find_data.cFileName);
        if (name != "." && name != "..") {
            files.push_back(createFileInfo(name, find_data));
        }
    } while (FindNextFile(handle, &find_data));

    FindClose(handle);
#else
    // Unix implementation
    if (search_path.empty()) {
        search_path = ".";
    }

    DIR* dir = opendir(search_path.c_str());
    if (!dir) {
        throw std::runtime_error("Failed to list directory: " + path);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name(entry->d_name);
        if (name != "." && name != "..") {
            // Get full path for stat
            std::string full_path = search_path;
            if (full_path.back() != '/') {
                full_path += "/";
            }
            full_path += name;

            struct stat stat_buf;
            if (stat(full_path.c_str(), &stat_buf) == 0) {
                files.push_back(createFileInfo(name, stat_buf));
            }
        }
    }

    closedir(dir);
#endif

    return files;
}

// Change directory
bool FileSystem::changeDirectory(std::string& current_path, const std::string& target) {
    if (target.empty()) {
        return false;
    }

    std::string new_path;
    
    if (target == "..") {
        new_path = getParentDirectory(current_path);
    } else if (target == "~") {
#ifdef _WIN32
        const char* home = getenv("USERPROFILE");
#else
        const char* home = getenv("HOME");
#endif
        if (home) {
            new_path = std::string(home);
        } else {
            return false;
        }
    } else if (target[0] == '/' || (target.length() > 1 && target[1] == ':')) {
        // Absolute path
        new_path = target;
    } else {
        // Relative path
        new_path = current_path;
        if (new_path.empty()) {
            new_path = ".";
        }
#ifdef _WIN32
        if (new_path.back() != '\\' && new_path.back() != '/') {
            new_path += "\\";
        }
#else
        if (new_path.back() != '/') {
            new_path += "/";
        }
#endif
        new_path += target;
    }

    if (isDirectory(new_path)) {
        current_path = getAbsolutePath(new_path);
        return true;
    }

    return false;
}

// Get parent directory
std::string FileSystem::getParentDirectory(const std::string& path) {
    // Handle root directory cases first
    if (path == "/") {
        return "/";
    }
    
    if (path == "\\") {
        return "\\";
    }

    if (path.empty()) {
        return "";
    }

    std::string parent = path;
    
    // Remove trailing slashes
    while (!parent.empty() && (parent.back() == '/' || parent.back() == '\\')) {
        parent.pop_back();
    }

    if (parent.empty()) {
        return "";
    }

#ifdef _WIN32
    // Handle Windows drive letters
    if (parent.length() >= 2 && parent[1] == ':' && 
        (parent.length() == 2 || parent.find_first_of("/\\", 2) == std::string::npos)) {
        // This is a drive letter, return it as is
        return parent;
    }
    
    if (parent.length() == 3 && parent[1] == ':' && (parent[2] == '\\' || parent[2] == '/')) {
        // This is a root drive directory (e.g., "C:\")
        return parent;
    }
    
    // Special case for root directories with trailing slash already removed
    if (parent.length() == 2 && parent[1] == ':') {
        return parent;
    }
#else
    // On Unix systems, we've already handled the root directory case above
#endif

    // Find last slash
    size_t pos = parent.find_last_of("/\\");
    if (pos == std::string::npos) {
        return "."; // Current directory
    }

    if (pos == 0) {
#ifdef _WIN32
        // On Windows, return the drive letter with backslash
        if (parent.length() > 1 && parent[1] == ':') {
            return parent.substr(0, 3); // e.g., "C:\\"
        }
        return parent.substr(0, 1) + "\\"; // Root directory with backslash
#else
        // On Unix, root directory is "/"
        return "/";
#endif
    }

    // Add debugging output
    // std::cout << "getParentDirectory: path=" << path << ", parent=" << parent << ", pos=" << pos << std::endl;
    
    return parent.substr(0, pos);
}

// Get absolute path
std::string FileSystem::getAbsolutePath(const std::string& path) {
    if (path.empty()) {
        return "";
    }

#ifdef _WIN32
    char abs_path[MAX_PATH];
    if (_fullpath(abs_path, path.c_str(), MAX_PATH) != nullptr) {
        return std::string(abs_path);
    }
    return path;
#else
    char abs_path[PATH_MAX];
    if (realpath(path.c_str(), abs_path) != nullptr) {
        return std::string(abs_path);
    }
    return path;
#endif
}

// Check if path is directory
bool FileSystem::isDirectory(const std::string& path) {
    if (path.empty()) {
        return false;
    }

#ifdef _WIN32
    DWORD attrs = GetFileAttributes(path.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
    struct stat stat_buf;
    if (stat(path.c_str(), &stat_buf) != 0) {
        return false;
    }
    return S_ISDIR(stat_buf.st_mode);
#endif
}

// Check if file exists
bool FileSystem::fileExists(const std::string& path) {
    if (path.empty()) {
        return false;
    }

#ifdef _WIN32
    DWORD attrs = GetFileAttributes(path.c_str());
    return attrs != INVALID_FILE_ATTRIBUTES;
#else
    struct stat stat_buf;
    return stat(path.c_str(), &stat_buf) == 0;
#endif
}
