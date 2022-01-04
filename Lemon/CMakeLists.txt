cmake_minimum_required(VERSION 3.22)

set(lemon_VERSION_MAJOR 0)
set(lemon_VERSION_MINOR 0)
set(lemon_VERSION_PATCH 0)
set(lemon_VERSION ${lemon_VERSION_MAJOR}.${lemon_VERSION_MINOR}.${lemon_VERSION_PATCH})

set(lemon_DESC "2D game engine written in C++")

# ------ ENTT, GLAD, STB --------------
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


project(Lemon DESCRIPTION ${lemon_DESC} VERSION ${lemon_VERSION} LANGUAGES C CXX)

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
add_compile_definitions(LEMON_RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

add_library(lemon)
add_subdirectory(src)
add_library(lemon::lemon ALIAS lemon)

target_link_libraries(lemon PUBLIC 
	glad 
	glfw
	glm
	${OPENAL_LIBRARY} 
	stb 
	EnTT::EnTT 
	${CMAKE_DL_LIBS}
)

target_include_directories(lemon PUBLIC include)

set_target_properties(lemon PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)
target_precompile_headers(lemon PRIVATE
	<string>
	<utility>
	<algorithm>
	<functional>
	[["lemon/core/assert.h"]]
	[["lemon/core/logger.h"]]
)
target_compile_definitions(lemon PUBLIC
	"$<$<CONFIG:Debug>:LEMON_DEBUG>"
	"${LEMON_PLATFORM}"
)
target_compile_options(lemon PUBLIC 
	"$<$<AND:$<CXX_COMPILER_ID:ARMCland,AppleClang,Clang,GNU>,$<CONFIG:DEBUG>>:-Wall;-Wextra;-Wformat=2;-Wunused>"
	"$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:DEBUG>>:-W3>"
)