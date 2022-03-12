project(LemonPhysics)

set(lemon_physics_SRC
	src/physics_engine.cpp
)

add_library(lemon_physics ${lemon_physics_SRC})
add_library(lemon::physics ALIAS lemon_physics)

target_include_directories(lemon_physics PUBLIC include)
target_link_libraries(lemon_physics PUBLIC
	lemon::core
)
