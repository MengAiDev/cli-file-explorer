#ifndef COMMANDLINEINTERFACE_H
#define COMMANDLINEINTERFACE_H

#include "FileSystem.h"
#include <string>

class CommandLineInterface {
public:
    CommandLineInterface();
    void start();

private:
    void showPrompt();
    void processCommand(const std::string& command);
    void listDirectory();
    void changeDirectory(const std::string& path);
    void showHelp();
    
    FileSystem fileSystem;
    std::string currentPath;
};

#endif // COMMANDLINEINTERFACE_H
