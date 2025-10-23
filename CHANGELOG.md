# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Comprehensive test suite with unit tests
- GitHub Actions CI/CD pipeline
- Security scanning and static analysis
- Professional documentation and examples
- Code quality tools and formatting
- Performance benchmarks

### Changed
- Enhanced security with input validation
- Improved error handling
- Better documentation structure

## [1.0.0] - 2024-01-01

### Added
- Initial release of CardID library
- Support for major card networks (Visa, Mastercard, Amex, Discover)
- Luhn algorithm validation
- Network detection from card numbers
- Command-line interface
- C library API
- Cross-platform support (Linux, macOS, Windows)
- CMake build system
- Zero external dependencies

### Features
- **Card Networks Supported**:
  - Visa (prefix 4, lengths 13, 16, 19)
  - Mastercard (prefixes 51-55, 2221-2720, length 16)
  - American Express (prefixes 34, 37, length 15)
  - Discover (prefixes 6011, 622126-622925, 644-649, 65, lengths 16, 19)

- **Security Features**:
  - Buffer overflow protection
  - Input validation and sanitization
  - Memory safety (no dynamic allocation)
  - No logging or storage of sensitive data

- **Performance**:
  - < 1KB stack usage
  - ~1μs per validation on modern hardware
  - < 10KB compiled library
  - Zero external dependencies

### API
```c
// Core functions
bool cardid_luhn_digits(const char* digits, int len);
cardid_extract_result cardid_extract_digits(const char* input, char* out_digits, int out_capacity);
cardid_network cardid_detect_network(const char* digits, int len);
void cardid_analyze(const char* input, cardid_result* out, cardid_extract_result* extract_meta);
```

### CLI Usage
```bash
# Validate a card number
./cardid 4111-1111-1111-1111
# Output: VISA

# Interactive mode
./cardid
# Number: 5555-5555-5555-4444
# Output: MASTERCARD
```

### Installation
```bash
# Build from source
cmake -S . -B build
cmake --build build --parallel
sudo cmake --install build
```

## [0.1.0] - 2023-12-01

### Added
- Initial development version
- Basic Luhn validation
- Simple network detection
- Minimal CLI interface

### Changed
- Refactored for better API design
- Improved error handling
- Enhanced documentation

## [0.0.1] - 2023-11-01

### Added
- Initial proof of concept
- Basic card validation functionality
- Simple test cases
