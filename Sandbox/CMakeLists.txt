cmake_minimum_required(VERSION 3.22)

project(Sandbox DESCRIPTION "Test project" LANGUAGES CXX)

set(sandbox_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_system.cpp)

add_executable(sandbox ${sandbox_SRC})
target_link_libraries(sandbox PUBLIC lemon::lemon)

target_compile_definitions(
    sandbox PUBLIC
    "ASSET_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/assets\""
)

set(SYSTEMS_LIST
    "src/input_system.h"
)

foreach(path IN LISTS SYSTEMS_LIST)
    set(filename)
    get_filename_component(filename ${path} NAME_WE)
    set(output "${CMAKE_BINARY_DIR}/_generated/${filename}.json")
    add_custom_command(
        OUTPUT ${output}
        DEPENDS ${path}
        COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/export_systems.py "${output}" "${CMAKE_CURRENT_SOURCE_DIR}/${path}"
        COMMENT "Generated editor data for ${filename}"
    )
    set(GAME_TYPES ${GAME_TYPES} ${output})
endforeach(path in LISTS ARGN)
set(GAME_TYPES ${GAME_TYPES} PARENT_SCOPE)

add_custom_target(lemon_game_types
  ALL DEPENDS
    ${GAME_TYPES}
)

