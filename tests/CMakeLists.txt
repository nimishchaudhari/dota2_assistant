# CMakeLists.txt for tests

cmake_minimum_required(VERSION 3.20)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0  # Using a more recent tag with proper versioning
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Set policies for GTest
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)

FetchContent_MakeAvailable(googletest)

# Add test directories
add_subdirectory(unit)
add_subdirectory(integration)
add_subdirectory(performance)
