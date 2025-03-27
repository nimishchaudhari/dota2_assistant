# CMakeLists.txt for performance tests

# Performance test executable
add_executable(performance_tests)

target_sources(performance_tests PRIVATE
    # Add test source files here as they are created
)

target_link_libraries(performance_tests PRIVATE
    gtest_main
    benchmark::benchmark
    dota2_assistant_core
    dota2_assistant_services
    dota2_assistant_analysis
    dota2_assistant_ui
    dota2_assistant_utils
)

# Google Benchmark
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.1
)
FetchContent_MakeAvailable(benchmark)

include(GoogleTest)
gtest_discover_tests(performance_tests)
