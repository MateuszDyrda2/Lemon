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
add_compile_definitions(RIVER_RESOURCE_PATH="../../../resources/")

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
	${river_SRC_DIR}game/entity.cpp
	${river_SRC_DIR}game/scene.cpp
	${river_SRC_DIR}game/scene_manager.cpp
	${river_SRC_DIR}game/transform_system.cpp
	${river_SRC_DIR}event_system/event_handler.cpp
	${river_SRC_DIR}renderer/rendering_system.cpp
	${river_SRC_DIR}renderer/shader.cpp
	${river_SRC_DIR}renderer/batch_renderer.cpp
	${river_SRC_DIR}renderer/rendering_context.cpp
	${river_SRC_DIR}renderer/texture.cpp
	${river_SRC_DIR}renderer/vertex_buffer.cpp
	${river_SRC_DIR}renderer/vertex_array.cpp
	${river_SRC_DIR}time/clock.cpp
	${river_SRC_DIR}scripting/scripting_system.cpp
	${river_SRC_DIR}resources/resource_manager.cpp
	${river_SRC_DIR}services/services.cpp
	${river_SRC_DIR}input/input.cpp
)

add_library(river ${river_SRC})

find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(river PUBLIC glad glfw glm stb EnTT::EnTT ${CMAKE_DL_LIBS})

target_include_directories(river PUBLIC include)

target_compile_options(river PRIVATE 
	"$<$<AND:$<CXX_COMPILER_ID:ARMCland,AppleClang,Clang,GNU>,$<CONFIG:DEBUG>>:-Wall;-Wextra;-Wformat=2;-Wunused>"
	"$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:DEBUG>>:-W3>"
)
add_subdirectory(example)