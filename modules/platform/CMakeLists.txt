project(LemonPlatform)

set(lemon_platform_SRC
	src/window.cpp
	src/input.cpp
	src/glfw_devices.cpp
)

add_library(lemon_platform ${lemon_platform_SRC})
add_library(lemon::platform ALIAS lemon_platform)

target_include_directories(lemon_platform PUBLIC include/)
target_link_libraries(lemon_platform 
	PUBLIC
	lemon::core
	lemon::events
	PRIVATE
	glad
	glfw
)