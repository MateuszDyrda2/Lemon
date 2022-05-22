project(LemonPhysics)

set(lemon_physics_SRC
	src/BVH_tree.cpp
	src/SAT.cpp
	src/systems/physics_system.cpp
	src/systems/collision_system.cpp
	src/systems/collision_response_system.cpp
	src/systems/movement_system.cpp
	src/systems/prephysics_system.cpp
	src/systems/postphysics_system.cpp
)

add_library(lemon_physics ${lemon_physics_SRC})
add_library(lemon::physics ALIAS lemon_physics)

target_include_directories(lemon_physics PUBLIC include)
target_link_libraries(lemon_physics PUBLIC
	lemon::core
	lemon::scene
)
