cmake_minimum_required(VERSION 3.22)

# IMGUI 
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/)
# AssetManager
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tools/)

project(Lemonade)

add_compile_definitions(LEMONADE_RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
set(lemonade_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/ )
set(lemonade_SRC
	${lemonade_SRC_DIR}main.cpp
	${lemonade_SRC_DIR}application.cpp
	${lemonade_SRC_DIR}editor_window.cpp
	${lemonade_SRC_DIR}editor_system.cpp
)

add_executable(lemonade ${lemonade_SRC})
target_include_directories(lemonade PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(lemonade PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

target_link_libraries(lemonade PUBLIC lemon::lemon lemon::assetManager imgui)