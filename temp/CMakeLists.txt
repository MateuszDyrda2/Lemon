
cmake_minimum_required(VERSION 3.22)

project(temp LANGAUGES CXX)

set(temp_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(COMPONENTS_LIST )

add_components("${COMPONENTS_LIST}")

target_compile_definitions(
    temp PUBLIC
    "ASSET_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/assets\""
    "SCENE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/scenes\""
    "SETTINGS_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/temp.lmnproj\""
)

