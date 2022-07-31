project(LemonEngine)

set(engine_SRC
	src/engine.cpp
)
add_library(lemon_engine ${engine_SRC})
add_library(lemon::engine ALIAS lemon_engine)
target_include_directories(lemon_engine PUBLIC include/)

target_link_libraries(lemon_engine 
	PUBLIC lemon::core
	PUBLIC lemon::platform	
	PUBLIC lemon::assets
	PUBLIC lemon::threading
	PUBLIC lemon::rendering
	PUBLIC lemon::scripting
)