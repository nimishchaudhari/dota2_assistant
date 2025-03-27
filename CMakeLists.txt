# Root CMakeLists.txt for the Dota 2 AI Assistant project

cmake_minimum_required(VERSION 3.20)
project(dota2_assistant VERSION 0.1.0 LANGUAGES CXX)

# Set CMake policies
cmake_policy(SET CMP0135 NEW)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include FetchContent for dependency management
include(FetchContent)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Include single-header library for JSON
include_directories(${CMAKE_SOURCE_DIR}/external/include)

# Create the single-header libraries directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/external/include/nlohmann)

# Download nlohmann/json as a single header file
file(DOWNLOAD
    https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
    ${CMAKE_SOURCE_DIR}/external/include/nlohmann/json.hpp
    SHOW_PROGRESS
)

# Download GoogleTest
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Make googletest available
FetchContent_MakeAvailable(googletest)

# Add source directories
add_subdirectory(src)

# Main executable
add_executable(dota2_assistant src/main.cpp)

target_link_libraries(dota2_assistant PRIVATE
    dota2_assistant_core
    dota2_assistant_services
    dota2_assistant_utils
)

# Enable testing
enable_testing()
add_subdirectory(tests)
