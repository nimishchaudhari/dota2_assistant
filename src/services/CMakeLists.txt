# CMakeLists.txt for services components

add_library(dota2_assistant_services STATIC)

target_sources(dota2_assistant_services PRIVATE
    # Add source files here as they are created
)

target_include_directories(dota2_assistant_services PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dota2_assistant_services PUBLIC
    dota2_assistant_core
    # Add other dependencies here
)
