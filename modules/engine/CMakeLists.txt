project(LemonEngine)

set(lemon_engine_SRC 
	src/engine.cpp
	src/systems/input_system.cpp
	src/systems/debug_system.cpp
	src/py_bindings/py_core.cpp
	src/py_bindings/py_assets.cpp
	src/py_bindings/py_audio.cpp
	src/py_bindings/py_events.cpp
	src/py_bindings/py_input.cpp
	src/py_bindings/py_scene.cpp
	src/py_bindings/py_threading.cpp
	src/py_bindings/py_platform.cpp
	src/py_bindings/py_rendering.cpp
	src/py_bindings/py_systems.cpp
)

add_library(lemon_engine ${lemon_engine_SRC})
add_library(lemon::engine ALIAS lemon_engine)

target_include_directories(lemon_engine PUBLIC include/)
target_link_libraries(lemon_engine
	PUBLIC
	lemon::core
	lemon::threading
	lemon::events
	lemon::platform
	lemon::scene
	lemon::rendering
	lemon::audio
	lemon::physics
	lemon::assets
	lemon::scripting
	imgui
)
