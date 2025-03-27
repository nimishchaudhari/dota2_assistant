# CMakeLists.txt for services components

add_library(dota2_assistant_services STATIC)

target_sources(dota2_assistant_services PRIVATE
    gsi_connector.cpp
    gsi_connector.h
)

target_include_directories(dota2_assistant_services PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dota2_assistant_services PUBLIC
    dota2_assistant_core
    dota2_assistant_utils
    nlohmann_json::nlohmann_json
)

# Windows specific libraries
if(WIN32)
    target_link_libraries(dota2_assistant_services PRIVATE
        advapi32 # For registry access
    )
endif()
