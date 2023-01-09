cmake_minimum_required(VERSION 3.22)

project(Sandbox DESCRIPTION "Test project" LANGUAGES CXX)

set(sandbox_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/movement_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/attack_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/enemy_system.cpp
)

set(COMPONENTS_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/src/player_components.h"
)

add_components("${COMPONENTS_LIST}")

add_executable(sandbox ${sandbox_SRC} ${serialization_files})
target_link_libraries(sandbox PUBLIC lemon::lemon)


target_compile_definitions(
    sandbox PUBLIC
    "ASSET_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/assets\""
    "SCENE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/scenes\""
    "SETTINGS_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/sandbox.lmnproj\""
)