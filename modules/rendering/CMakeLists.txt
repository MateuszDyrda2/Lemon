project(LemonRendering)

set(lemon_rendering_SRC
	src/basic_renderer.cpp
	src/batch_renderer.cpp
	src/framebuffer.cpp
	src/rendering_context.cpp
	src/shader.cpp
	src/texture.cpp
	src/vertex_array.cpp
	src/vertex_buffer.cpp
	src/systems/rendering_system.cpp
)
add_library(lemon_rendering ${lemon_rendering_SRC})
add_library(lemon::rendering ALIAS lemon_rendering)

target_include_directories(lemon_rendering PUBLIC include/)
target_link_libraries(lemon_rendering 
	PUBLIC
	lemon::core
	lemon::assets
	lemon::platform
	lemon::events
	lemon::scene
	glad
	PRIVATE
	stb
)