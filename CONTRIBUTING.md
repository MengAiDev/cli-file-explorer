# Contributing to CLI File Explorer

Thank you for your interest in contributing to the CLI File Explorer project! This document provides guidelines and information to help you contribute effectively.

## Getting Started

1. Fork the repository on GitHub
2. Clone your fork locally
3. Create a new branch for your feature or bug fix
4. Make your changes
5. Test your changes thoroughly
6. Commit your changes with a clear, descriptive commit message
7. Push your changes to your fork
8. Submit a pull request to the main repository

## Development Setup

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Git

### Building the Project

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running Tests

```bash
cd build
ctest
```

## Code Style

- Follow the existing code style in the project
- Use meaningful variable and function names
- Comment your code where necessary, especially for complex logic
- Keep functions small and focused on a single task
- Use const correctness where appropriate

## Project Structure

- `src/` - Source code files
- `include/` - Header files
- `tests/` - Unit tests
- `docs/` - Documentation
- `debian/` - Debian packaging files
- `CMakeLists.txt` - CMake build configuration

## Submitting Changes

- Ensure your code follows the project's coding standards
- Write clear, concise commit messages
- Include tests for new functionality
- Update documentation as needed
- Verify all tests pass before submitting a pull request

## Reporting Issues

- Use the GitHub issue tracker to report bugs or suggest features
- Include as much relevant information as possible
- For bugs, include steps to reproduce, expected behavior, and actual behavior
- For feature requests, explain the use case and benefits

## Code of Conduct

Please note that this project is released with a Contributor Code of Conduct. By participating in this project you agree to abide by its terms.

## Questions?

If you have any questions about contributing, feel free to open an issue or contact the maintainers.
