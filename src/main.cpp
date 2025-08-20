#include "cli/CommandLineInterface.h"
#include "bookmark/BookMark.h"
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Check for non-interactive commands
    if (argc > 1) {
        // Load bookmarks first
        BookMark::loadBookMarks();
        
        std::string command = argv[1];
        
        if (command == "bookmark") {
            if (argc > 2) {
                std::string subcommand = argv[2];
                
                if (subcommand == "add" && argc > 3) {
                    std::string path = argv[3];
                    // Make relative paths absolute
                    if (path[0] != '/') {
                        char* cwd = get_current_dir_name();
                        path = std::string(cwd) + "/" + path;
                        free(cwd);
                    }
                    if (BookMark::addBookMark(path)) {
                        std::cout << "Bookmark added: " << path << std::endl;
                    } else {
                        std::cout << "Bookmark already exists: " << path << std::endl;
                    }
                } else if (subcommand == "remove" && argc > 3) {
                    std::string path = argv[3];
                    // Make relative paths absolute
                    if (path[0] != '/') {
                        char* cwd = get_current_dir_name();
                        path = std::string(cwd) + "/" + path;
                        free(cwd);
                    }
                    if (BookMark::removeBookMark(path)) {
                        std::cout << "Bookmark removed: " << path << std::endl;
                    } else {
                        std::cout << "Bookmark not found: " << path << std::endl;
                    }
                } else if (subcommand == "list") {
                    std::cout << "Bookmarks:" << std::endl;
                    for (const auto& bookmark : BookMark::bookMarks) {
                        std::cout << "  " << bookmark << std::endl;
                    }
                } else if (subcommand == "clear") {
                    if (BookMark::emptyBookMarks()) {
                        std::cout << "All bookmarks cleared." << std::endl;
                    } else {
                        std::cout << "No bookmarks to clear." << std::endl;
                    }
                } else {
                    std::cout << "Invalid bookmark command. Use 'bookmark add <path>', 'bookmark remove <path>', 'bookmark list', or 'bookmark clear'." << std::endl;
                    return 1;
                }
                
                // Save bookmarks after modification
                BookMark::saveBookMarks();
                return 0;
            } else {
                std::cout << "Usage: " << argv[0] << " bookmark [add|remove|list|clear] [path]" << std::endl;
                return 1;
            }
        } else {
            std::cout << "Unknown command: " << command << std::endl;
            std::cout << "Available non-interactive commands: bookmark" << std::endl;
            return 1;
        }
    }
    
    // Start interactive mode
    CommandLineInterface cli;
    cli.start();
    
    return 0;
}
