project(LemonEvents)

set(lemon_events_SRC
	src/event_bus.cpp
)

add_library(lemon_events ${lemon_events_SRC})
add_library(lemon::events ALIAS lemon_events)

target_include_directories(lemon_events PUBLIC include)
target_link_libraries(lemon_events
	PUBLIC
	lemon::core
	lemon::threading
)