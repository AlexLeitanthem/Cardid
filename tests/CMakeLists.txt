# Test configuration
cmake_minimum_required(VERSION 3.20)

# Find required packages
find_package(Threads REQUIRED)

# Create test executable
add_executable(test_cardid test_cardid.c)

# Link with cardid library
target_link_libraries(test_cardid PRIVATE cardid)

# Include directories
target_include_directories(test_cardid PRIVATE ../include)

# Add test to CTest
enable_testing()
add_test(NAME cardid_tests COMMAND test_cardid)

# Set test properties
set_tests_properties(cardid_tests PROPERTIES
    TIMEOUT 30
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
