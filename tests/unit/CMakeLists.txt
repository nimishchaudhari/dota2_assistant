# CMakeLists.txt for unit tests

# Unit test executable
add_executable(unit_tests
    main.cpp
    test_gsi_connector.cpp
    test_game_state.cpp
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    GTest::gmock
    dota2_assistant_core
    dota2_assistant_services
    dota2_assistant_utils
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
