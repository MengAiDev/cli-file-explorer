#ifndef COMMANDLINEINTERFACE_H
#define COMMANDLINEINTERFACE_H

#include "FileSystem.h"
#include <string>
#include <vector>

#ifdef USE_NCURSES
#include <ncurses.h>
#endif

class CommandLineInterface {
public:
    CommandLineInterface();
    void start();
    void startInteractive();

private:
    void showPrompt();
    void processCommand(const std::string& command);
    void executeShellCommand(const std::string& command);
    void listDirectory();
    void changeDirectory(const std::string& path);
    void createFile(const std::string& filename);
    void deleteFile(const std::string& path);
    void deleteDirectory(const std::string& path);
    void renameFile(const std::string& oldPath, const std::string& newPath);
    void copyFile(const std::string& sourcePath, const std::string& destinationPath);
    void showHelp();
    
#ifdef USE_NCURSES
    void interactiveListDirectory();
    void displayFileContent(const std::string& filePath);
    void editFileWithVim(const std::string& filePath);
#endif
    
    FileSystem fileSystem;
    std::string currentPath;
};

#endif // COMMANDLINEINTERFACE_H
