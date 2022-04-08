cmake_minimum_required(VERSION 3.22)

set(lemon_VERSION_MAJOR 0)
set(lemon_VERSION_MINOR 0)
set(lemon_VERSION_PATCH 1)
set(lemon_VERSION ${lemon_VERSION_MAJOR}.${lemon_VERSION_MINOR}.${lemon_VERSION_PATCH})

set(lemon_DESC "2D game engine written in C++")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
project(Lemon DESCRIPTION ${lemon_DESC} VERSION ${lemon_VERSION} LANGUAGES C CXX)

# - ENTT, GLAD, STB, RAPIDJSON, AudioFile
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/)
# -------------------------------------
# ---------------OPENAL----------------
find_package(OpenAL REQUIRED)
# -------------------------------------
# --------------- GLFW ----------------
find_package(glfw3 3.3 REQUIRED)
# -------------------------------------
# --------------- GLM -----------------
find_package(glm REQUIRED)
# -------------------------------------


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(LEMON_PROFILE "Profile functions" OFF)

if(WIN32)
	#platform windows
	set(LEMON_PLATFORM LEMON_WINDOWS)
elseif(APPLE)
	#platform apple
	set(LEMON_PLATFORM LEMON_APPLE)
elseif(UNIX)
	#platform linux
	set(LEMON_PLATFORM LEMON_LINUX)
endif(WIN32)

add_compile_definitions(
	"$<$<CONFIG:Debug>:LEMON_DEBUG>"
	"${LEMON_PLATFORM}"
	"$<$<BOOL:${LEMON_PROFILE}>:LEMON_PROFILE=1>"
)
add_compile_options(
	"$<$<AND:$<CXX_COMPILER_ID:ARMCland,AppleClang,Clang,GNU>,$<CONFIG:DEBUG>>:-Wall;-Wextra;-Wformat=2;-Wunused;-fvisibility=hidden>"
	"$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:DEBUG>>:-W3;-Zc:preprocessor>"
)


add_subdirectory(modules/core)
add_subdirectory(modules/threading)
add_subdirectory(modules/events)
add_subdirectory(modules/platform)
add_subdirectory(modules/rendering)
add_subdirectory(modules/audio)
add_subdirectory(modules/physics)
add_subdirectory(modules/assets)
add_subdirectory(modules/scripting)
add_subdirectory(modules/scene)
add_subdirectory(modules/engine)
add_subdirectory(modules/serialization)

option(LEMON_BUILD_EXAMPLE "Build example" ON)
option(LEMON_BUILD_LEMONADE "Build lemonade editor" OFF)
option(LEMON_BUILD_TOOLS "Build engine tools" OFF)
option(LEMON_BUILD_TESTS "Build lemon tests" ON)

if(LEMON_BUILD_EXAMPLE)
	message("Building example")
	add_subdirectory(example)
endif()
if(LEMON_BUILD_LEMONADE)
	message("Building Lemonade")
	add_subdirectory(lemonade)
endif()
if(LEMON_BUILD_TOOLS)
	message("Building tools")
	add_subdirectory(tools)
endif()
if(LEMON_BUILD_TESTS)
	message("Building tests")
	add_subdirectory(tests)
endif()
