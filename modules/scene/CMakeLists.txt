project(LemonScene)

set(lemon_scene_SRC
	src/entity.cpp
	src/scene.cpp
	src/scene_manager.cpp
	src/reflection.cpp
)

add_library(lemon_scene ${lemon_scene_SRC})
add_library(lemon::scene ALIAS lemon_scene)

target_include_directories(lemon_scene PUBLIC include/)
target_link_libraries(lemon_scene
	PUBLIC
	lemon::core
	lemon::audio
	lemon::threading
	lemon::events
	lemon::assets
	lemon::rendering
	EnTT::EnTT
)