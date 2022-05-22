project(LemonCore)

# set source files
set(lemon_core_SRC 
	src/logger.cpp
	src/string_id.cpp
	src/time/game_clock.cpp
	src/file.cpp
	src/game.cpp 
	src/instrumentor.cpp
)

add_library(lemon_core ${lemon_core_SRC})
add_library(lemon::core ALIAS lemon_core)

target_include_directories(lemon_core PUBLIC include)
target_link_libraries(lemon_core 
	PRIVATE 
	glm
	rapidjson
)

