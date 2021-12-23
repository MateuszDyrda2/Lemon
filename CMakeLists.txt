cmake_minimum_required(VERSION 3.20)

set(river_VERSION_MAJOR 0)
set(river_VERSION_MINOR 0)
set(river_VERSION_PATCH 0)
set(river_VERSION ${river_VERSION_MAJOR}.${river_VERSION_MINOR}.${river_VERSION_PATCH})

set(river_DESC "2D game engine written in C++")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/)

project(river DESCRIPTION ${river_DESC} VERSION ${river_VERSION} LANGUAGES C CXX)

if(WIN32)
	#platform windows
	add_compile_definitions(RIVER_WINDOWS)
elseif(APPLE)
	#platform apple
	add_compile_definitions(RIVER_APPLE)
elseif(UNIX)
	#platform linux
	add_compile_definitions(RIVER_LINUX)
endif(WIN32)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DRIVER_DEBUG")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(river_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(river_SRC
	${river_SRC_DIR}engine.cpp
	${river_SRC_DIR}window/window.cpp
	${river_SRC_DIR}core/logger.cpp
	${river_SRC_DIR}core/string_id.cpp
	${river_SRC_DIR}game/object.cpp
	${river_SRC_DIR}game/component.cpp
	${river_SRC_DIR}game/entity.cpp
	${river_SRC_DIR}game/scene.cpp
)

add_library(river ${river_SRC})

target_link_libraries(river PRIVATE glfw glm)

target_include_directories(river PUBLIC include)

target_compile_options(river PRIVATE 
	"$<$<AND:$<CXX_COMPILER_ID:ARMCland,AppleClang,Clang,GNU>,$<CONFIG:DEBUG>>:-Wall;-Wextra;-Wformat=2;-Wunused>"
	"$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:DEBUG>>:-W3>"
)
add_subdirectory(example)