project(LemonPlatform)

set(lemon_platform_SRC
	src/window.cpp
	src/input.cpp
)
add_library(lemon_platform ${lemon_platform_SRC})
add_library(lemon::platform ALIAS lemon_platform)
target_include_directories(lemon_platform PUBLIC include/)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

add_subdirectory(libs/glfw)

target_link_libraries(lemon_platform
	PUBLIC lemon::core
	PUBLIC lemon::events
	PUBLIC glfw
)
