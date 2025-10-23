cmake_minimum_required(VERSION 3.20)
project(cardid C VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler options
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Build options
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_CLI "Build CLI executable" ON)

# Library
add_library(cardid STATIC src/cardid.c)
target_include_directories(cardid PUBLIC include)

# Set library properties
set_target_properties(cardid PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/cardid.h
)

# CLI executable
if(BUILD_CLI)
    add_executable(cardid_cli src/main.c)
    target_link_libraries(cardid_cli PRIVATE cardid)
    set_target_properties(cardid_cli PROPERTIES OUTPUT_NAME cardid)
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Benchmarks
option(BUILD_BENCHMARKS "Build benchmark suite" ON)
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# Installation
install(TARGETS cardid
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

if(BUILD_CLI)
    install(TARGETS cardid_cli
        RUNTIME DESTINATION bin
    )
endif()
