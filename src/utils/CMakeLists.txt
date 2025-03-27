# CMakeLists.txt for utility components

add_library(dota2_assistant_utils INTERFACE)

target_include_directories(dota2_assistant_utils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
