# CMakeLists.txt for tests

cmake_minimum_required(VERSION 3.20)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Add test directories
add_subdirectory(unit)
add_subdirectory(integration)
add_subdirectory(performance)
