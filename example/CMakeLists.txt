cmake_minimum_required(VERSION 3.22)

project(Sanbox)

add_executable(sandbox src/sandbox.cpp)

target_compile_definitions(sandbox PUBLIC EXAMPLE_ASSET_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets")
target_compile_definitions(sandbox PUBLIC EXAMPLE_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets.json")
target_compile_definitions(sandbox PUBLIC EXAMPLE_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(sandbox PUBLIC lemon::serialization)