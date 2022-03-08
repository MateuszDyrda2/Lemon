project(LemonEngine)

set(lemon_engine_SRC 
	src/engine.cpp
	src/systems/audio_system.cpp
	src/systems/rendering_system.cpp
	src/systems/scripting_system.cpp
	src/systems/transform_system.cpp
	src/py_bindings/py_core.cpp
)

add_library(lemon_engine ${lemon_engine_SRC})
add_library(lemon::engine ALIAS lemon_engine)

target_include_directories(lemon_engine PUBLIC include/)
target_link_libraries(lemon_engine
	PUBLIC
	lemon::core
	lemon::threading
	lemon::events
	lemon::window
	lemon::rendering
	lemon::audio
	#lemon::physics
	lemon::assets
	lemon::scene
	lemon::scripting
)