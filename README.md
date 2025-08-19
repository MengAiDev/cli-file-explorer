# CLI File Explorer

A cross-platform command-line file browser similar to Windows File Explorer, implemented in C++.

## Features

- Cross-platform support (Windows, Linux, macOS)
- Interactive command-line interface
- Directory navigation with `cd` command
- File and directory listing with `ls` command
- File type identification (directories, files, symlinks)
- Help system with `help` command
- Exit functionality with `exit` command
- Interactive file browsing mode (press 'i' to start)

## Advanced Features

- File operations (copy, move, delete, create directory)
- File search functionality with regex pattern matching
- File sorting by name, size, date, or type
- Support for both file name and content search

## Installation

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)
- ncurses library (for Linux - `sudo apt-get install libncurses-dev`)

### Building from Source

```bash
git clone https://github.com/MengAiDev/cli-file-explorer.git
cd cli-file-explorer
mkdir build
cd build
cmake ..
cmake --build .
```

### Running the Application

```bash
./cli_file_explorer
```

## Usage

Once the application is running, you can use the following commands:

- `ls` - List files in the current directory
- `cd <path>` - Change to the specified directory
- `help` - Display help information
- `i` - Start interactive mode (file browser with arrow keys)
- `exit` - Exit the application

### Interactive Mode

In interactive mode:
- Use up/down arrow keys to navigate through files and directories
- Press Enter to enter a directory or view a file's content
- Press 'q' to exit file view or interactive mode

When viewing file content:
- Line numbers are displayed in yellow (if terminal supports color)
- Content is displayed with proper wrapping
- Total line count is shown in the header
- Use up/down arrow keys to scroll through the file content

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
