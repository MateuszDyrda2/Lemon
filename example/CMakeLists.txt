cmake_minimum_required(VERSION 3.22)

project(Sanbox)

add_compile_definitions(EXAMPLE_ASSET_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets")
add_compile_definitions(EXAMPLE_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets.json")
add_compile_definitions(EXAMPLE_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(sandbox src/sandbox.cpp)

target_link_libraries(sandbox PUBLIC lemon::lemon)