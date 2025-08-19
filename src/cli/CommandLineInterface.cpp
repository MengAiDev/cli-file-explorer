#include "../../include/cli/CommandLineInterface.h"
#include "../../include/fileops/FileOperations.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <filesystem>

CommandLineInterface::CommandLineInterface() {
    currentPath = fileSystem.getAbsolutePath(".");
}

void CommandLineInterface::start() {
    std::cout << "CLI File Explorer\n";
    std::cout << "Type 'help' for available commands or 'exit' to quit.\n";
    std::cout << "Press 'i' to start interactive mode.\n\n";
    
    std::string input;
    while (true) {
        showPrompt();
        std::getline(std::cin, input);
        
        if (input == "exit") {
            break;
        }
        
        if (input == "i") {
            startInteractive();
            continue;
        }
        
        processCommand(input);
    }
}

void CommandLineInterface::showPrompt() {
    std::cout << "[" << currentPath << "]$ ";
}

void CommandLineInterface::processCommand(const std::string& command) {
    // Handle shell commands that start with '!'
    if (!command.empty() && command[0] == '!') {
        if (command.length() > 1) {
            executeShellCommand(command.substr(1));
        }
        return;
    }
    
    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    if (tokens.empty()) {
        return;
    }
    
    if (tokens[0] == "ls") {
        listDirectory();
    } else if (tokens[0] == "cd" && tokens.size() > 1) {
        changeDirectory(tokens[1]);
    } else if (tokens[0] == "help") {
        showHelp();
    } else if (tokens[0] == "touch" && tokens.size() > 1) {
        createFile(tokens[1]);
    } else if (tokens[0] == "rm" && tokens.size() > 1) {
        deleteFile(tokens[1]);
    } else if (tokens[0] == "rmdir" && tokens.size() > 1) {
        deleteDirectory(tokens[1]);
    } else if (tokens[0] == "mv" && tokens.size() > 2) {
        renameFile(tokens[1], tokens[2]);
    } else {
        std::cout << "Unknown command: " << command << "\n";
        std::cout << "Type 'help' for available commands.\n";
    }
}

void CommandLineInterface::listDirectory() {
    auto files = fileSystem.listDirectory(currentPath);
    
    // Sort files alphabetically
    std::sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
        return a.getName() < b.getName();
    });
    
    // Display header
    std::cout << std::left << std::setw(30) << "Name" 
              << std::setw(12) << "Size" 
              << std::setw(20) << "Modified" 
              << std::setw(12) << "Permissions" 
              << std::setw(10) << "Type" << "\n";
    std::cout << std::string(90, '-') << "\n";
    
    for (const auto& file : files) {
        std::string type;
        switch (file.getType()) {
            case FileInfo::FileType::DIRECTORY:
                type = "[DIR]";
                break;
            case FileInfo::FileType::FILE:
                type = "[FILE]";
                break;
            case FileInfo::FileType::SYMLINK:
                type = "[LINK]";
                break;
            default:
                type = "[UNK]";
                break;
        }
        
        std::string name = file.getName();
        // Truncate long names
        if (name.length() > 29) {
            name = name.substr(0, 26) + "...";
        }
        
        std::cout << std::left << std::setw(30) << name
                  << std::setw(12) << (file.getType() == FileInfo::FileType::DIRECTORY ? "<DIR>" : FileOperations::formatFileSize(file.getSize()))
                  << std::setw(20) << FileOperations::formatTime(file.getModifiedTime())
                  << std::setw(12) << FileOperations::formatPermissions(file.getPermissions())
                  << std::setw(10) << type << "\n";
    }
}

void CommandLineInterface::changeDirectory(const std::string& path) {
    std::string newPath = currentPath;
    
    if (!fileSystem.changeDirectory(newPath, path)) {
        std::cout << "Directory does not exist: " << path << "\n";
    } else {
        currentPath = newPath;
    }
}

void CommandLineInterface::createFile(const std::string& filename) {
    std::string filePath = currentPath;
    
    // Add appropriate path separator
#ifdef _WIN32
    if (filePath.back() != '\\' && filePath.back() != '/') {
        filePath += "\\";
    }
#else
    if (filePath.back() != '/') {
        filePath += "/";
    }
#endif
    
    filePath += filename;
    
    if (FileOperations::createFile(filePath)) {
        std::cout << "File created successfully: " << filePath << "\n";
    } else {
        std::cout << "Failed to create file: " << filePath << "\n";
    }
}

void CommandLineInterface::executeShellCommand(const std::string& command) {
    int result = system(command.c_str());
    if (result != 0) {
        std::cout << "Shell command failed with exit code: " << result << "\n";
    }
}

void CommandLineInterface::showHelp() {
    std::cout << "Available commands:\n";
    std::cout << "  ls                - List files in current directory\n";
    std::cout << "  cd <path>         - Change directory\n";
    std::cout << "  touch <filename>  - Create a new file\n";
    std::cout << "  rm <file>         - Delete a file\n";
    std::cout << "  rmdir <directory> - Delete an empty directory\n";
    std::cout << "  mv <old> <new>    - Rename/move file or directory\n";
    std::cout << "  !<command>        - Execute shell command\n";
    std::cout << "  help              - Show this help message\n";
    std::cout << "  i                 - Start interactive mode\n";
    std::cout << "  exit              - Exit the program\n";
    std::cout << "\nIn interactive mode:\n";
    std::cout << "  ↑/↓               - Navigate lines\n";
    std::cout << "  PgUp/PgDn         - Page up/down\n";
    std::cout << "  Enter             - Open directories or view file content\n";
    std::cout << "  Ctrl+E            - Edit file with vim\n";
    std::cout << "  q                 - Quit interactive mode\n";
}

void CommandLineInterface::startInteractive() {
#ifdef USE_NCURSES
    interactiveListDirectory();
#else
    std::cout << "Interactive mode is not available. Please compile with ncurses support.\n";
#endif
}