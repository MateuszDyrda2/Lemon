project(LemonScripting)

set(lemon_scripting_SRC
	src/domain.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(mono-2 REQUIRED mono-2)

add_library(lemon_scripting ${lemon_scripting_SRC})
add_library(lemon::scripting ALIAS lemon_scripting)
target_include_directories(lemon_scripting 
	PUBLIC include/
	PUBLIC "${mono-2_INCLUDE_DIRS}"
)
message("lib = ${mono-2_INCLUDE_DIRS}")
message("lib = ${mono-2_LIBRARIES}")

target_link_libraries(lemon_scripting
	PUBLIC lemon::core
	PUBLIC "${mono-2_LIBRARIES}"
)