cmake_minimum_required(VERSION 3.22)

# ---------- Top level CMake file -----------
project(LemonEngine)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Build sandbox 
option(LEMON_BUILD_SANDBOX "Build sandbox" ON)
# Build tests
option(LEMON_BUILD_TESTS "Build lemon tests" ON)
# -------------------------------------------

# ------------------ Python --------------------
find_package(Python COMPONENTS Interpreter)
# ----------------------------------------------
set(ENGINE_TYPES)
set(GAME_TYPES)
# Must be in the top level CMakeLists file
enable_testing()
# --------- Add Game engine project ---------
add_subdirectory(Lemon)
# -------------------------------------------

if(LEMON_BUILD_SANDBOX)
	message("Building sandbox")
	add_subdirectory(Sandbox)
endif()
if(LEMON_BUILD_TESTS)
	message("Building tests")
	add_subdirectory(tests)
endif()
# -------------------------------------------

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/_generated)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/_generated/types.json
    DEPENDS ${ENGINE_TYPES} ${GAME_TYPES}
    COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/write_component_file.py "${CMAKE_BINARY_DIR}/_generated" "${CMAKE_BINARY_DIR}/_generated/types.json"
    COMMENT "Generated types.json"
)
add_custom_target(lemon_generated 
  ALL DEPENDS 
    ${CMAKE_BINARY_DIR}/_generated/types.json
    ${ENGINE_TYPES}
    ${GAME_TYPES}
)
set_source_files_properties(${CMAKE_BINARY_DIR}/_generated/types.json
    PROPERTIES GENERATED TRUE)
add_dependencies(lemon_generated lemon_engine_types lemon_game_types)

