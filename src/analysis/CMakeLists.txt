# CMakeLists.txt for analysis components

add_library(dota2_assistant_analysis STATIC)

target_sources(dota2_assistant_analysis PRIVATE
    placeholder.cpp
)

target_include_directories(dota2_assistant_analysis PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dota2_assistant_analysis PUBLIC
    dota2_assistant_core
    # Add other dependencies here
)
