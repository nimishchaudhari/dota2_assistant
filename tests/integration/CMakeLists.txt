# CMakeLists.txt for integration tests

# Integration tests will be added later
# For now, adding a placeholder to ensure CMake configuration works

# Add a placeholder executable so CMake doesn't error out
add_executable(integration_tests_placeholder EXCLUDE_FROM_ALL
    placeholder.cpp
)

target_link_libraries(integration_tests_placeholder PRIVATE
    GTest::gtest
    GTest::gmock
)
