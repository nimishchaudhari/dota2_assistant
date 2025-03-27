# CMakeLists.txt for unit tests

include(GoogleTest)

add_executable(unit_tests
    test_game_state.cpp
    test_gsi_connector.cpp
)

target_link_libraries(unit_tests PRIVATE
    gtest_main
    gmock_main
    dota2_assistant_core
    dota2_assistant_services
    dota2_assistant_utils
)

# Register tests with CTest
gtest_discover_tests(unit_tests)
