#ifndef COMMAND_LINE_INTERFACE_H
#define COMMAND_LINE_INTERFACE_H

#include <string>
#include <vector>
#include "../include/FileSystem.h"

class CommandLineInterface {
private:
    FileSystem fileSystem;
    std::string currentPath;

public:
    CommandLineInterface();
    void start();
    void showPrompt();
    void processCommand(const std::string& command);
    void listDirectory();
    void changeDirectory(const std::string& path);
    void showHelp();
};

#endif // COMMAND_LINE_INTERFACE_H
