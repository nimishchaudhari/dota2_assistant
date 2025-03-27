# CMakeLists.txt for unit tests

# Unit test executable
add_executable(unit_tests)

target_sources(unit_tests PRIVATE
    # Add test source files here as they are created
)

target_link_libraries(unit_tests PRIVATE
    gtest_main
    dota2_assistant_core
    dota2_assistant_services
    dota2_assistant_analysis
    dota2_assistant_ui
    dota2_assistant_utils
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
