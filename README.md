# CLI File Explorer

A cross-platform command-line interface file browser similar to Windows File Explorer, built with C++17.

## Features

- Cross-platform support (Windows, Linux, macOS)
- Interactive command-line interface
- Directory navigation (cd, ls)
- File operations (copy, move, delete, create directory)
- File search functionality with regex pattern matching
- File sorting by name, size, date, or type
- File type detection and display

## Installation

### From Source

1. Clone the repository:
   ```bash
   git clone https://github.com/MengAiDev/cli-file-explorer.git
   cd cli-file-explorer
   ```

2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. Run the application:
   ```bash
   ./cli_file_explorer
   ```

### From Packages

#### Debian/Ubuntu (APT)

```bash
# Download the .deb package from releases
sudo dpkg -i cli-file-explorer_1.0.0_amd64.deb
```

#### Arch Linux (Pacman)

```bash
# Using yay or another AUR helper
yay -S cli-file-explorer

# Or manually from the PKGBUILD
makepkg -si
```

## Usage

After starting the application, you'll see a prompt showing your current directory:

```
/home/user:>
```

### Commands

- `ls` - List directory contents
- `cd <directory>` - Change directory
- `cp <source> <destination>` - Copy file or directory
- `mv <source> <destination>` - Move file or directory
- `rm <file>` - Delete file or directory
- `mkdir <directory>` - Create new directory
- `search <pattern>` - Search for files by name pattern
- `search-content <pattern>` - Search for files containing pattern
- `sort <criteria>` - Sort files (name, size, date, type)
- `help` - Show help information
- `exit` - Exit the application

### Examples

```bash
# List directory contents
ls

# Change to a subdirectory
cd documents

# Copy a file
cp file.txt file_backup.txt

# Search for files with ".cpp" extension
search ".*\.cpp"

# Sort files by size
sort size

# Exit the application
exit
```

## Building for Different Platforms

### Linux

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Windows

```cmd
mkdir build
cd build
cmake ..
cmake --build .
```

### macOS

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Contributing

Contributions are welcome! Please read our [Contributing Guidelines](CONTRIBUTING.md) for details on how to submit pull requests, report issues, and contribute to the project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Thanks to all contributors who have helped with the development of this project
- Inspired by Windows File Explorer and other CLI file managers
