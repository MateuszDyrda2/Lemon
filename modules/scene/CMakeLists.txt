project(LemonScene)

set(lemon_scene_SRC
	src/entity.cpp
	src/scene.cpp
	src/scene_manager.cpp
	src/reflection.cpp
	src/systems/entity_system.cpp
	src/systems/transform_system.cpp
	src/systems/event_system.cpp
	src/systems/time_system.cpp
)

add_library(lemon_scene ${lemon_scene_SRC})
add_library(lemon::scene ALIAS lemon_scene)

target_include_directories(lemon_scene PUBLIC include/)
target_link_libraries(lemon_scene
	PUBLIC
	lemon::core
	lemon::threading
	lemon::events
	EnTT::EnTT
)