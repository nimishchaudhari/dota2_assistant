# CMakeLists.txt for core components

add_library(dota2_assistant_core STATIC)

target_sources(dota2_assistant_core PRIVATE
    game_state.cpp
    game_state.h
)

target_include_directories(dota2_assistant_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/include
)

target_link_libraries(dota2_assistant_core PUBLIC
)
