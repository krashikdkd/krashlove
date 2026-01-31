# Contributing to KrashLove

Thank you for your interest in contributing to KrashLove Windows Optimizer!

## Development Setup

1. **Prerequisites**
   - Windows 10/11 (64-bit)
   - CMake 3.15 or higher
   - MSVC (Visual Studio 2019+) or MinGW-w64
   - Git

2. **Clone the repository**
   ```bash
   git clone https://github.com/krashikdkd/krashlove.git
   cd krashlove
   ```

3. **Build the project**
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```

## Coding Standards

### C++ Style Guide
- Use C++17 standard features
- Follow existing code style and formatting
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and single-purpose

### Naming Conventions
- Classes: PascalCase (e.g., `SystemCleanup`)
- Functions: PascalCase (e.g., `OptimizeRAM()`)
- Variables: camelCase (e.g., `processId`)
- Constants: UPPER_CASE (e.g., `MAX_PATH`)
- Namespaces: PascalCase (e.g., `KrashLove`)

### Code Organization
- Header files in `include/` directory
- Implementation files in `src/` directory
- One class per file when practical
- Use header guards (`#pragma once`)

## Pull Request Process

1. **Fork the repository** and create a feature branch
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes**
   - Write clean, readable code
   - Add comments where necessary
   - Test your changes thoroughly

3. **Commit your changes**
   ```bash
   git add .
   git commit -m "Add feature: brief description"
   ```

4. **Push to your fork**
   ```bash
   git push origin feature/your-feature-name
   ```

5. **Create a Pull Request**
   - Provide a clear description of changes
   - Reference any related issues
   - Ensure builds pass on Windows

## Testing Guidelines

### Manual Testing
- Test on both Windows 10 and Windows 11
- Test with administrator privileges
- Verify all menu options work correctly
- Check for memory leaks (use Task Manager)
- Test error handling with invalid inputs

### Safety Checks
- Ensure operations are reversible
- No modification of critical system files
- Proper error handling and user feedback
- Validate all user inputs
- Test with antivirus software enabled

## Feature Requests

When requesting a new feature:
1. Check if it already exists
2. Explain the use case
3. Describe the expected behavior
4. Consider performance and safety implications

## Bug Reports

When reporting bugs:
1. Describe the issue clearly
2. Include steps to reproduce
3. Specify Windows version
4. Include any error messages
5. Note if running as administrator

## Areas for Contribution

### High Priority
- Additional optimization modules
- Improved error handling
- Performance metrics and benchmarking
- Configuration file support
- Undo/restore functionality

### Medium Priority
- Logging system
- Scheduled optimization tasks
- GUI version (separate project)
- Additional language support
- Profile system (gaming, work, balanced)

### Low Priority
- Plugin system
- Remote management
- Web interface
- Multi-user support

## Code Review

All submissions require review. We use GitHub pull requests for this purpose.

Reviewers will check for:
- Code quality and style
- Functionality and correctness
- Safety and stability
- Performance impact
- Documentation completeness

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Questions?

Feel free to open an issue for any questions about contributing!

---

Thank you for contributing to KrashLove! 🚀
