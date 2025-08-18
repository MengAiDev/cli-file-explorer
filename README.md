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

## Advanced Features

- File operations (copy, move, delete, create directory)
- File search functionality with regex pattern matching
- File sorting by name, size, date, or type
- Support for both file name and content search

## Installation

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)

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
- `exit` - Exit the application

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
