#include "../../include/cli/CommandLineInterface.h"
#include "../../include/fileops/FileOperations.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

CommandLineInterface::CommandLineInterface() {
    currentPath = fileSystem.getAbsolutePath(".");
}

void CommandLineInterface::start() {
    std::cout << "CLI File Explorer\n";
    std::cout << "Type 'help' for available commands or 'exit' to quit.\n\n";
    
    std::string input;
    while (true) {
        showPrompt();
        std::getline(std::cin, input);
        
        if (input == "exit") {
            break;
        }
        
        processCommand(input);
    }
}

void CommandLineInterface::showPrompt() {
    std::cout << "[" << currentPath << "]$ ";
}

void CommandLineInterface::processCommand(const std::string& command) {
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
    
    for (const auto& file : files) {
        std::string type;
        switch (file.getType()) {
            case FileInfo::FileType::DIRECTORY:
                type = "[DIR] ";
                break;
            case FileInfo::FileType::FILE:
                type = "[FILE] ";
                break;
            case FileInfo::FileType::SYMLINK:
                type = "[LINK] ";
                break;
            default:
                type = "[UNK] ";
                break;
        }
        std::cout << type << file.getName() << "\n";
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

void CommandLineInterface::showHelp() {
    std::cout << "Available commands:\n";
    std::cout << "  ls          - List files in current directory\n";
    std::cout << "  cd <path>   - Change directory\n";
    std::cout << "  touch <filename> - Create a new file\n";
    std::cout << "  help        - Show this help message\n";
    std::cout << "  exit        - Exit the program\n";
}
