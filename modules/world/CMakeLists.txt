project(LemonWorld)

set(lemon_world_SRC
	src/scene.cpp
)

add_library(lemon_world ${lemon_world_SRC})
add_library(lemon::world ALIAS lemon_world)
target_include_directories(lemon_world PUBLIC include/)

add_subdirectory(libs/entt)
target_link_libraries(lemon_world
	PUBLIC lemon::core
	PUBLIC EnTT::EnTT
)