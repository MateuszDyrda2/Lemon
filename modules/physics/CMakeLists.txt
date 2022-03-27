project(LemonPhysics)

set(lemon_physics_SRC
	src/BVH_tree.cpp
	src/SAT.cpp
)

add_library(lemon_physics ${lemon_physics_SRC})
add_library(lemon::physics ALIAS lemon_physics)

target_include_directories(lemon_physics PUBLIC include)
target_link_libraries(lemon_physics PUBLIC
	lemon::core
)
