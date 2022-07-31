cmake_minimum_required(VERSION 3.20)

set(LEMON_VERSION_MAJOR 0)
set(LEMON_VERSION_MINOR 1)
set(LEMON_VERSION_PATCH 0)
set(LEMON_VERSION ${LEMON_VERSION_MAJOR}.${LEMON_VERSION_MINOR}.${LEMON_VERSION_PATCH})

project(Lemon VERSION ${LEMON_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	#platform windows
	set(LEMON_PLATFORM LEMON_WINDOWS)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
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
    "$<$<BOOL:${LEMON_PROFILE}>:LEMON_PROFILE>"
)
add_compile_options(
    "$<$<AND:$<CXX_COMPILER_ID:ARMCland,AppleClang,Clang,GNU>,$<CONFIG:DEBUG>>:-Wall;-Wextra;-Wformat=2;-Wunused;-fvisibility=hidden>"
    "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:DEBUG>>:-W3;-wd5105;-Zc:preprocessor>"
)
# module definitions
# CORE
add_subdirectory(modules/core)
# EVENTS
add_subdirectory(modules/events)
# THREADING
add_subdirectory(modules/threading)
# PLATFORM
add_subdirectory(modules/platform)
# ASSETS
add_subdirectory(modules/assets)
# RENDERING
add_subdirectory(modules/rendering)
# AUDIO
add_subdirectory(modules/audio)
# SCENE
add_subdirectory(modules/world)
# ENGINE
add_subdirectory(modules/engine)

option(Lemon_BUILD_SANDBOX "Build sandbox" ON)
option(Lemon_BUILD_TESTS "Build tests" ON)

if(${Lemon_BUILD_SANDBOX})
	add_subdirectory(sandbox)
	message("Building sandbox")
endif()

if(${Lemon_BUILD_TESTS})
	add_subdirectory(tests)
	message("Building tests")
endif()



	 
