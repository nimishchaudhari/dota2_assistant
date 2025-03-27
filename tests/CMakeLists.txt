# CMakeLists.txt for tests

cmake_minimum_required(VERSION 3.20)

# Set policies for GTest
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)

# Add test directories
add_subdirectory(unit)
add_subdirectory(integration)
add_subdirectory(performance)
