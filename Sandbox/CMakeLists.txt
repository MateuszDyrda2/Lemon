cmake_minimum_required(VERSION 3.22)

project(Sandbox DESCRIPTION "Test project" LANGUAGES CXX)

set(sandbox_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/movement_system.cpp

)

set(COMPONENTS_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/src/player_components.h"
)

list(APPEND COMPONENTS_LIST ${COMPONENTS_LIST_LEMON})

foreach(path IN LISTS COMPONENTS_LIST)
    set(filename)
    get_filename_component(filename ${path} NAME_WE)
    set(output "${CMAKE_BINARY_DIR}/_generated/${filename}.json")
    add_custom_command(
        OUTPUT ${output}
        DEPENDS ${path}
        COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/export_components.py "${output}" "${path}"
        COMMENT "Generated editor data for ${filename}")
    set(GAME_TYPES ${GAME_TYPES} ${output})
endforeach(path in LISTS ARGN)

add_custom_target(lemon_game_types
  ALL DEPENDS
    ${GAME_TYPES}
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/_generated)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/_generated/types.json
    DEPENDS ${GAME_TYPES}
    COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/write_component_file.py "${CMAKE_BINARY_DIR}/_generated" "${CMAKE_BINARY_DIR}/_generated/types.json"
    COMMENT "Generated types.json"
)

add_custom_target(lemon_generated 
  ALL DEPENDS 
    ${CMAKE_BINARY_DIR}/_generated/types.json
    ${GAME_TYPES}
)
set_source_files_properties(${CMAKE_BINARY_DIR}/_generated/types.json
    PROPERTIES GENERATED TRUE)
add_dependencies(lemon_generated lemon_game_types)

string(REPLACE ";" " " INCLUDE_PATHS "${COMPONENTS_LIST_LEMON};${COMPONENTS_LIST}")
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/_generated/types.cpp
    DEPENDS ${CMAKE_BINARY_DIR}/_generated/types.json ${GAME_TYPES}
    COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/create_reflection.py 
        "${CMAKE_BINARY_DIR}/_generated/types.cpp" "${CMAKE_BINARY_DIR}/_generated/types.json"
        "${INCLUDE_PATHS}"
    COMMENT "Generated types.cpp"
)

add_executable(sandbox ${sandbox_SRC} ${CMAKE_BINARY_DIR}/_generated/types.cpp)
target_link_libraries(sandbox PUBLIC lemon::lemon)

target_compile_definitions(
    sandbox PUBLIC
    "ASSET_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/assets\""
)
