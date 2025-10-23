# Contributing to CardID

Thank you for your interest in contributing to CardID! This document provides guidelines and information for contributors.

## 🤝 How to Contribute

### Reporting Issues

Before creating an issue, please:

1. **Search existing issues** to avoid duplicates
2. **Check the documentation** to ensure it's not a usage question
3. **Verify the issue** with the latest version

When creating an issue, please include:

- **Description**: Clear description of the problem
- **Steps to reproduce**: Detailed steps to reproduce the issue
- **Expected behavior**: What you expected to happen
- **Actual behavior**: What actually happened
- **Environment**: OS, compiler version, build configuration
- **Additional context**: Any other relevant information

### Security Issues

**DO NOT** create public issues for security vulnerabilities. Instead:

1. Email security details to: alexleitanthem0@icloud.com
2. Include steps to reproduce and potential impact
3. We'll respond within 48 hours

### Feature Requests

We welcome feature requests! Please:

1. **Check existing issues** to avoid duplicates
2. **Describe the use case** and why it would be valuable
3. **Provide examples** of how the feature would be used
4. **Consider implementation complexity** and maintenance burden

## 🛠️ Development Setup

### Prerequisites

- **C Compiler**: GCC 4.9+, Clang 3.5+, or MSVC 2015+
- **CMake**: Version 3.20 or higher
- **Git**: For version control
- **Development Tools**: Valgrind, cppcheck (optional but recommended)

### Getting Started

```bash
# Fork and clone the repository
git clone https://github.com/your-username/cardid.git
cd cardid

# Create a development branch
git checkout -b feature/your-feature-name

# Build the project
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel

# Run tests
cmake --build build --target test
```

### Development Dependencies

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential cmake valgrind cppcheck
```

#### macOS
```bash
brew install cmake cppcheck
```

#### Windows
- Install Visual Studio with C++ development tools
- Install CMake from cmake.org
- Install Git for Windows

## 📝 Coding Standards

### C Code Style

We follow these coding standards:

- **Indentation**: 4 spaces (no tabs)
- **Line length**: 100 characters maximum
- **Naming**: snake_case for functions and variables, UPPER_CASE for constants
- **Comments**: Use `/* */` for block comments, `//` for line comments
- **Documentation**: Document all public functions with Doxygen-style comments

### Example Code Style

```c
/**
 * @brief Validates a payment card number using Luhn algorithm
 * @param digits The card number as a string of digits
 * @param len The length of the digit string
 * @return true if valid, false otherwise
 */
bool cardid_luhn_digits(const char* digits, int len) {
    // Implementation here
    return true;
}
```

### Code Quality Requirements

- **No warnings**: Code must compile without warnings
- **Memory safety**: No buffer overflows, use-after-free, or memory leaks
- **Input validation**: All public functions must validate inputs
- **Error handling**: Proper error handling and return codes
- **Documentation**: Public APIs must be documented

## 🧪 Testing

### Running Tests

```bash
# Run all tests
cmake --build build --target test

# Run specific test
./build/tests/test_cardid

# Run with memory checking (Linux)
valgrind --leak-check=full ./build/tests/test_cardid
```

### Writing Tests

When adding new features, include comprehensive tests:

1. **Unit tests** for individual functions
2. **Integration tests** for complete workflows
3. **Edge case tests** for boundary conditions
4. **Error condition tests** for invalid inputs

### Test Structure

```c
static int test_new_feature() {
    printf("\n=== Testing New Feature ===\n");
    
    // Test valid cases
    TEST_ASSERT(condition, "Description of what should happen");
    
    // Test invalid cases
    TEST_ASSERT(!condition, "Description of what should not happen");
    
    TEST_PASS("New feature tests");
    return 0;
}
```

## 📋 Pull Request Process

### Before Submitting

1. **Run tests**: Ensure all tests pass
2. **Check code style**: Follow the coding standards
3. **Update documentation**: Update relevant documentation
4. **Add tests**: Include tests for new functionality
5. **Update CHANGELOG**: Document changes in CHANGELOG.md

### Pull Request Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Tests pass locally
- [ ] New tests added for new functionality
- [ ] All existing tests still pass

## Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] Documentation updated
- [ ] No breaking changes (or breaking changes documented)
```

### Review Process

1. **Automated checks** must pass (CI/CD, static analysis)
2. **Code review** by maintainers
3. **Testing** on multiple platforms
4. **Documentation** review
5. **Security review** for sensitive changes

## 🏗️ Architecture Guidelines

### Library Design

- **Minimal dependencies**: Avoid external dependencies
- **Simple API**: Easy to use and understand
- **Performance**: Optimize for speed and memory usage
- **Security**: Security-first design principles
- **Portability**: Cross-platform compatibility

### Adding New Features

1. **Design first**: Plan the API and implementation
2. **Backward compatibility**: Don't break existing APIs
3. **Documentation**: Document all new public APIs
4. **Testing**: Include comprehensive tests
5. **Performance**: Consider performance implications

### File Organization

```
cardid/
├── include/          # Public headers
├── src/             # Source files
├── tests/            # Test files
├── examples/         # Example code
├── docs/             # Documentation
└── tools/            # Build tools and scripts
```

## 🐛 Debugging

### Common Issues

1. **Build failures**: Check CMake version and compiler
2. **Test failures**: Run with debug output for more information
3. **Memory issues**: Use Valgrind or AddressSanitizer
4. **Performance issues**: Profile with appropriate tools

### Debug Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
```

### Memory Debugging

```bash
# Linux with Valgrind
valgrind --leak-check=full --show-leak-kinds=all ./build/tests/test_cardid

# With AddressSanitizer
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_FLAGS="-fsanitize=address"
```

## 📚 Documentation

### Code Documentation

- **Function comments**: Use Doxygen-style comments for public functions
- **Inline comments**: Explain complex logic
- **README updates**: Update README.md for user-facing changes
- **API documentation**: Keep API documentation current

### Documentation Standards

```c
/**
 * @brief Brief description of the function
 * @param param1 Description of parameter 1
 * @param param2 Description of parameter 2
 * @return Description of return value
 * @note Any important notes
 * @warning Any warnings
 */
```

## 🚀 Release Process

### Version Numbering

We use [Semantic Versioning](https://semver.org/):

- **MAJOR**: Breaking changes
- **MINOR**: New features (backward compatible)
- **PATCH**: Bug fixes (backward compatible)

### Release Checklist

1. **Update version** in CMakeLists.txt
2. **Update CHANGELOG.md** with new features and fixes
3. **Run full test suite** on all supported platforms
4. **Update documentation** if needed
5. **Create release tag** following versioning scheme
6. **Publish release** with release notes

## 💡 Tips for Contributors

### Getting Help

- **GitHub Discussions**: For questions and discussions
- **GitHub Issues**: For bug reports and feature requests
- **Code review**: Learn from code review feedback
- **Documentation**: Read existing documentation and examples

### Best Practices

1. **Start small**: Begin with small, focused changes
2. **Test thoroughly**: Test your changes extensively
3. **Follow conventions**: Stick to established patterns
4. **Ask questions**: Don't hesitate to ask for clarification
5. **Be patient**: Code review can take time

### Common Mistakes to Avoid

- **Breaking changes** without proper documentation
- **Missing tests** for new functionality
- **Poor error handling** or input validation
- **Inconsistent code style**
- **Incomplete documentation**
