project(LemonScripting)

set(lemon_scripting_SRC
	src/py_engine.cpp
	src/py_script.cpp
	src/scriptable_entity.cpp
)

add_library(lemon_scripting ${lemon_scripting_SRC})
add_library(lemon::scripting ALIAS lemon_scripting)

target_include_directories(lemon_scripting PUBLIC include/ )
target_link_libraries(lemon_scripting 
	PUBLIC
	lemon::core
	lemon::assets
	lemon::scene
	pybind11::embed
	pybind11::module
	pybind11::pybind11
)