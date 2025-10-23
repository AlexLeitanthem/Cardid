# Benchmark configuration
cmake_minimum_required(VERSION 3.20)

# Find required packages
find_package(Threads REQUIRED)

# Create benchmark executable
add_executable(benchmark_cardid benchmark_cardid.c)

# Link with cardid library
target_link_libraries(benchmark_cardid PRIVATE cardid)

# Include directories
target_include_directories(benchmark_cardid PRIVATE ../include)

# Set benchmark properties
set_target_properties(benchmark_cardid PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)
