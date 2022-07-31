project(LemonThreading)

set(lemon_threading_SRC
	src/scheduler.cpp
)

add_library(lemon_threading ${lemon_threading_SRC})
add_library(lemon::threading ALIAS lemon_threading)
target_include_directories(lemon_threading PUBLIC include/)

add_subdirectory(libs/concurrentqueue)
target_link_libraries(lemon_threading
	PUBLIC lemon::core
	PUBLIC concurrentqueue
)