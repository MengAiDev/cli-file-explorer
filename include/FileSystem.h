#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>
#include <chrono>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#endif

class FileInfo {
public:
    enum class FileType {
        FILE,
        DIRECTORY,
        SYMLINK,
        UNKNOWN
    };

    FileInfo(const std::string& name, FileType type, size_t size,
             std::chrono::system_clock::time_point modified_time,
             uint32_t permissions);

    const std::string& getName() const;
    FileType getType() const;
    size_t getSize() const;
    std::chrono::system_clock::time_point getModifiedTime() const;
    uint32_t getPermissions() const;

private:
    std::string name_;
    FileType type_;
    size_t size_;
    std::chrono::system_clock::time_point modified_time_;
    uint32_t permissions_;
};

class FileSystem {
public:
    static std::vector<FileInfo> listDirectory(const std::string& path);
    static bool changeDirectory(std::string& current_path, const std::string& target);
    static std::string getParentDirectory(const std::string& path);
    static std::string getAbsolutePath(const std::string& path);
    static bool isDirectory(const std::string& path);
    static bool fileExists(const std::string& path);

private:
    static FileInfo createFileInfo(const std::string& name,
#ifdef _WIN32
                                  WIN32_FIND_DATA& find_data
#else
                                  struct stat& stat_buf
#endif
                                  );
};

#endif // FILESYSTEM_H
