# CMakeLists.txt for core components

add_library(dota2_assistant_core STATIC)

target_sources(dota2_assistant_core PRIVATE
    # Add source files here as they are created
)

target_include_directories(dota2_assistant_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dota2_assistant_core PUBLIC
    # Add dependencies here
)
