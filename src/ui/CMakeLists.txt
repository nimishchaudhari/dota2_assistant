# CMakeLists.txt for UI components

add_library(dota2_assistant_ui STATIC)

target_sources(dota2_assistant_ui PRIVATE
    # Add source files here as they are created
)

target_include_directories(dota2_assistant_ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dota2_assistant_ui PUBLIC
    dota2_assistant_core
    # Add other dependencies here
)
