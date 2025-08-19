#ifdef USE_NCURSES
#include "../../include/cli/CommandLineInterface.h"
#include "../../include/fileops/FileOperations.h"
#include <ncurses.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

void CommandLineInterface::interactiveListDirectory() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Hide cursor
    
    int selected = 0;
    int offset = 0;
    int maxRows = LINES - 3; // Leave space for header and footer
    
    while (true) {
        // Clear screen
        clear();
        
        // Get directory contents
        auto files = fileSystem.listDirectory(currentPath);
        
        // Sort files alphabetically
        std::sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
            // Directories first, then files
            if (a.getType() == FileInfo::FileType::DIRECTORY && b.getType() != FileInfo::FileType::DIRECTORY) {
                return true;
            }
            if (a.getType() != FileInfo::FileType::DIRECTORY && b.getType() == FileInfo::FileType::DIRECTORY) {
                return false;
            }
            return a.getName() < b.getName();
        });
        
        // Add parent directory entry if not at root
        if (currentPath != "/" && currentPath != ".") {
            std::vector<FileInfo> filesWithParent;
            filesWithParent.emplace_back("..", FileInfo::FileType::DIRECTORY, 0, 
                                         std::chrono::system_clock::time_point(), 0);
            filesWithParent.insert(filesWithParent.end(), files.begin(), files.end());
            files = std::move(filesWithParent);
        }
        
        // Calculate display bounds
        maxRows = LINES - 5; // Adjust for header and footer lines
        if (selected >= offset + maxRows) {
            offset = selected - maxRows + 1;
        } else if (selected < offset) {
            offset = selected;
        }
        
        // Display header
        printw("CLI File Explorer - Interactive Mode\n");
        printw("Current path: %s\n", currentPath.c_str());
        printw("%-30s %-12s %-20s %-12s %-10s\n", "Name", "Size", "Modified", "Permissions", "Type");
        printw("%s\n", std::string(90, '-').c_str());
        
        // Display files
        int end = std::min(offset + maxRows, (int)files.size());
        for (int i = offset; i < end; i++) {
            const auto& file = files[i];
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
            
            std::string sizeStr = (file.getType() == FileInfo::FileType::DIRECTORY) ? 
                                  "<DIR>" : FileOperations::formatFileSize(file.getSize());
            
            if (i == selected) {
                attron(A_REVERSE);
                printw("> %-30s %-12s %-20s %-12s %-10s\n", 
                       name.c_str(), 
                       sizeStr.c_str(),
                       FileOperations::formatTime(file.getModifiedTime()).c_str(),
                       FileOperations::formatPermissions(file.getPermissions()).c_str(),
                       type.c_str());
                attroff(A_REVERSE);
            } else {
                printw("  %-30s %-12s %-20s %-12s %-10s\n", 
                       name.c_str(), 
                       sizeStr.c_str(),
                       FileOperations::formatTime(file.getModifiedTime()).c_str(),
                       FileOperations::formatPermissions(file.getPermissions()).c_str(),
                       type.c_str());
            }
        }
        
        // Display footer
        printw("%s\n", std::string(90, '-').c_str());
        printw("↑/↓: Navigate lines | PgUp/PgDn: Page up/down | Enter: Open | Ctrl+E: Edit with vim | n: New file | q: Quit\n");
        
        // Refresh screen
        refresh();
        
        // Get user input
        int ch = getch();
        
        switch (ch) {
            case KEY_UP:
                if (selected > 0) {
                    selected--;
                }
                break;
                
            case KEY_DOWN:
                if (selected < (int)files.size() - 1) {
                    selected++;
                }
                break;
                
            case '\n': // Enter key
                {
                    const auto& selectedFile = files[selected];
                    if (selectedFile.getType() == FileInfo::FileType::DIRECTORY) {
                        if (selectedFile.getName() == "..") {
                            // Go to parent directory
                            currentPath = fileSystem.getParentDirectory(currentPath);
                            selected = 0;
                            offset = 0;
                        } else {
                            // Enter subdirectory
                            std::string newPath = currentPath;
#ifdef _WIN32
                            if (newPath.back() != '\\' && newPath.back() != '/') {
                                newPath += "\\";
                            }
#else
                            if (newPath.back() != '/') {
                                newPath += "/";
                            }
#endif
                            newPath += selectedFile.getName();
                            if (fileSystem.isDirectory(newPath)) {
                                currentPath = newPath;
                                selected = 0;
                                offset = 0;
                            }
                        }
                    } else if (selectedFile.getType() == FileInfo::FileType::FILE) {
                        // Display file content
                        std::string filePath = currentPath;
#ifdef _WIN32
                        if (filePath.back() != '\\' && filePath.back() != '/') {
                            filePath += "\\";
                        }
#else
                        if (filePath.back() != '/') {
                            filePath += "/";
                        }
#endif
                        filePath += selectedFile.getName();
                        displayFileContent(filePath);
                        
                        // After viewing file, refresh the directory listing
                        clear();
                        refresh();
                    }
                }
                break;
                
            case 5: // Ctrl+E
                {
                    const auto& selectedFile = files[selected];
                    if (selectedFile.getType() == FileInfo::FileType::FILE) {
                        // Edit file with vim
                        std::string filePath = currentPath;
#ifdef _WIN32
                        if (filePath.back() != '\\' && filePath.back() != '/') {
                            filePath += "\\";
                        }
#else
                        if (filePath.back() != '/') {
                            filePath += "/";
                        }
#endif
                        filePath += selectedFile.getName();
                        editFileWithVim(filePath);
                        
                        // After editing file, refresh the directory listing
                        clear();
                        refresh();
                    }
                }
                break;
                
            case 'n':
                {
                    // Create new file
                    // End ncurses mode to get user input
                    endwin();
                    
                    // Get filename from user
                    std::cout << "Enter filename: ";
                    std::string filename;
                    std::getline(std::cin, filename);
                    
                    // Create full path
                    std::string filePath = currentPath;
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
                    
                    // Create the file
                    if (FileOperations::createFile(filePath)) {
                        std::cout << "File created successfully: " << filePath << std::endl;
                    } else {
                        std::cout << "Failed to create file: " << filePath << std::endl;
                    }
                    
                    std::cout << "Press Enter to continue...";
                    std::cin.get();
                    
                    // Reinitialize ncurses
                    initscr();
                    cbreak();
                    noecho();
                    keypad(stdscr, TRUE);
                    curs_set(0); // Hide cursor
                    
                    // Refresh the directory listing
                    clear();
                    refresh();
                }
                break;

            case 'q':
            case 'Q':
                // Exit interactive mode
                endwin();
                return;
        }
    }
    
    // Clean up ncurses
    endwin();
}

void CommandLineInterface::displayFileContent(const std::string& filePath) {
    // Create a new window for file content
    WINDOW* fileWin = newwin(LINES - 2, COLS - 2, 1, 1);
    box(fileWin, 0, 0);
    wrefresh(fileWin);
    
    // Read file content
    std::ifstream file(filePath);
    if (!file.is_open()) {
        mvwprintw(fileWin, 1, 1, "Cannot open file: %s", filePath.c_str());
        wrefresh(fileWin);
        getch();
        delwin(fileWin);
        return;
    }
    
    // Display file content with scrolling
    int offset = 0;
    std::vector<std::string> lines;
    std::string line;
    
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    file.close();
    
    // Initialize color if possible
    bool hasColor = has_colors();
    if (hasColor) {
        start_color();
        init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Line numbers
        init_pair(2, COLOR_WHITE, COLOR_BLACK);  // Content
    }
    
    // Process lines for wrapping
    struct WrappedLine {
        int originalLineNum; // 0-based index in original lines vector
        std::string content;
    };
    
    std::vector<WrappedLine> wrappedLines;
    int contentStart = 9; // Position after line number
    int availableWidth = COLS - contentStart - 2;
    
    for (size_t i = 0; i < lines.size(); i++) {
        const std::string& originalLine = lines[i];
        
        // Handle empty lines
        if (originalLine.empty()) {
            wrappedLines.push_back({static_cast<int>(i), ""});
            continue;
        }
        
        // Handle line wrapping
        if ((int)originalLine.length() <= availableWidth) {
            // Line fits in one line
            wrappedLines.push_back({static_cast<int>(i), originalLine});
        } else {
            // Line needs to be wrapped
            size_t pos = 0;
            while (pos < originalLine.length()) {
                // Try to wrap at word boundaries
                size_t wrapPos = pos + availableWidth;
                if (wrapPos >= originalLine.length()) {
                    // Remaining text fits in one line
                    wrappedLines.push_back({static_cast<int>(i), originalLine.substr(pos)});
                    break;
                }
                
                // Try to find a word boundary
                size_t spacePos = originalLine.rfind(' ', wrapPos);
                if (spacePos != std::string::npos && spacePos > pos) {
                    // Found a space to wrap at
                    wrappedLines.push_back({static_cast<int>(i), originalLine.substr(pos, spacePos - pos)});
                    pos = spacePos + 1; // Skip the space
                } else {
                    // No good word boundary, hard wrap
                    wrappedLines.push_back({static_cast<int>(i), originalLine.substr(pos, availableWidth)});
                    pos += availableWidth;
                }
            }
        }
    }
    
    int ch;
    while (true) {
        werase(fileWin);
        box(fileWin, 0, 0);
        mvwprintw(fileWin, 0, 2, "File: %s (%d lines)", filePath.c_str(), (int)lines.size());
        
        // Display content with scrolling and line numbers
        int maxDisplayLines = LINES - 4;
        int end = std::min(offset + maxDisplayLines, (int)wrappedLines.size());
        
        for (int i = offset; i < end; i++) {
            const WrappedLine& wrappedLine = wrappedLines[i];
            
            // Format line number (only for first line of wrapped content)
            char lineNum[16];
            if (i == 0 || wrappedLine.originalLineNum != wrappedLines[i-1].originalLineNum) {
                snprintf(lineNum, sizeof(lineNum), "%6d ", wrappedLine.originalLineNum + 1);
            } else {
                snprintf(lineNum, sizeof(lineNum), "       "); // Just spaces for continuation lines
            }
            
            // Print line number with color if available
            if (hasColor) {
                wattron(fileWin, COLOR_PAIR(1));
                mvwprintw(fileWin, i - offset + 1, 1, "%s", lineNum);
                wattroff(fileWin, COLOR_PAIR(1));
            } else {
                mvwprintw(fileWin, i - offset + 1, 1, "%s", lineNum);
            }
            
            // Print content
            if (hasColor) {
                wattron(fileWin, COLOR_PAIR(2));
                mvwprintw(fileWin, i - offset + 1, contentStart, "%s", wrappedLine.content.c_str());
                wattroff(fileWin, COLOR_PAIR(2));
            } else {
                mvwprintw(fileWin, i - offset + 1, contentStart, "%s", wrappedLine.content.c_str());
            }
        }
        
        mvwprintw(fileWin, LINES - 3, 1, "Lines %d-%d of %d (wrapped)", offset + 1, end, (int)wrappedLines.size());
        mvwprintw(fileWin, LINES - 3, COLS - 25, "↑/↓: Navigate | q: Close");
        
        wrefresh(fileWin);
        
        ch = wgetch(fileWin);
        switch (ch) {
            case KEY_UP:
                if (offset > 0) {
                    offset--;
                }
                break;
                
            case KEY_DOWN:
                if (offset + maxDisplayLines < (int)wrappedLines.size()) {
                    offset++;
                }
                break;
                
            case KEY_PPAGE: // Page Up
                offset = std::max(0, offset - maxDisplayLines);
                break;
                
            case KEY_NPAGE: // Page Down
                if (offset + maxDisplayLines < (int)wrappedLines.size()) {
                    offset = std::min(offset + maxDisplayLines, (int)wrappedLines.size() - maxDisplayLines);
                }
                break;
                
            case 'q':
            case 'Q':
                delwin(fileWin);
                return;
        }
    }
}

void CommandLineInterface::editFileWithVim(const std::string& filePath) {
    // End ncurses mode before launching vim
    endwin();
    
    // Construct the vim command
    std::string command = "vim \"" + filePath + "\"";
    
    // Execute vim
    int result = system(command.c_str());
    
    // Reinitialize ncurses after vim exits
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Hide cursor
    
    // Check if vim command was successful
    if (result != 0) {
        // Display error message
        printw("Failed to open file with vim. Exit code: %d\n", result);
        printw("Press any key to continue...");
        refresh();
        getch();
    }
}
#endif // USE_NCURSES
