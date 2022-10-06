cmake_minimum_required(VERSION 3.22)

project(Sandbox DESCRIPTION "Test project" LANGUAGES CXX)

set(sandbox_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(sandbox ${sandbox_SRC})
target_link_libraries(sandbox PUBLIC lemon::lemon)

target_compile_features(sandbox PUBLIC cxx_std_20)
set_target_properties(sandbox PROPERTIES CXX_STANDARD_REQUIRED ON)

target_compile_definitions(
    sandbox PUBLIC
    "ASSET_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/assets\""
)

