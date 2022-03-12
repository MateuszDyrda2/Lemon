project(LemonWindow)

set(lemon_window_SRC
	src/glfw/glfw_window.cpp
	src/glfw/glfw_input.cpp
)

add_library(lemon_window ${lemon_window_SRC})
add_library(lemon::window ALIAS lemon_window)

target_include_directories(lemon_window PUBLIC include/)
target_link_libraries(lemon_window 
	PUBLIC
	lemon::core
	lemon::events
	PRIVATE
	glad
	glfw
)