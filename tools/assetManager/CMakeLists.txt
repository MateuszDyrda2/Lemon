cmake_minimum_required(VERSION 3.20)

project(AssetManager)

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/rapidxml)

add_library(assetManager asset_manager.cpp)
add_library(lemon::assetManager ALIAS assetManager)

target_link_libraries(assetManager PUBLIC rapidxml)
target_include_directories(assetManager PUBLIC .)