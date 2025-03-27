# Root CMakeLists.txt for the Dota 2 AI Assistant project

cmake_minimum_required(VERSION 3.20)
project(dota2_assistant VERSION 0.1.0 LANGUAGES CXX)

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

# Option to use local/embedded dependencies to avoid download issues
option(USE_EMBEDDED_DEPENDENCIES "Use embedded dependencies instead of downloading" ON)

# External dependencies - nlohmann_json
if(USE_EMBEDDED_DEPENDENCIES)
    # Use embedded version of nlohmann_json
    message(STATUS "Using embedded version of nlohmann_json")
    
    # This is a simplified version that just defines the target
    # without actually downloading the library
    add_library(nlohmann_json INTERFACE)
    target_include_directories(nlohmann_json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/include)
    
    # Create alias to match FetchContent convention
    add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
    
    # Make sure the external directory exists
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/include/nlohmann)
    
    # We'll need to download the single_include version manually
    message(STATUS "Please run scripts/download_dependencies.bat to get required libraries")
else()
    # Try to find an installed version
    find_package(nlohmann_json 3.11.2 QUIET)
    if(NOT nlohmann_json_FOUND)
        # Download from GitHub
        message(STATUS "Downloading nlohmann_json from GitHub")
        set(JSON_ImplicitConversions OFF CACHE BOOL "Enable implicit conversions" FORCE)
        set(JSON_MultipleHeaders ON CACHE BOOL "Use non-amalgamated version of the library" FORCE)
        
        FetchContent_Declare(
            nlohmann_json
            GIT_REPOSITORY https://github.com/nlohmann/json.git
            GIT_TAG v3.11.2
            CMAKE_POLICY_DEFAULT_CMP0077 NEW
        )
        
        # Set nlohmann_json's cmake_minimum_required version to match our project
        set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
        set(NLOHMANN_JSON_CMAKE_MINIMUM_REQUIRED_VERSION 3.20)
        
        FetchContent_MakeAvailable(nlohmann_json)
    endif()
endif()

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
