# CMakeLists.txt for integration tests

# Integration test executable
add_executable(integration_tests)

target_sources(integration_tests PRIVATE
    # Add test source files here as they are created
)

target_link_libraries(integration_tests PRIVATE
    gtest_main
    dota2_assistant_core
    dota2_assistant_services
    dota2_assistant_analysis
    dota2_assistant_ui
    dota2_assistant_utils
)

include(GoogleTest)
gtest_discover_tests(integration_tests)
