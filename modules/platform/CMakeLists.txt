project(LemonPlatform)

set(lemon_platform_SRC
	src/glfw/glfw_window.cpp
	src/glfw/glfw_input.cpp
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