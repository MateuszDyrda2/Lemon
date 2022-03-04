project(LemonSerialization)

set(lemon_serialization_SRC
	src/scene_serializer.cpp
	src/detail/basic_types_serializer.cpp
)

add_library(lemon_serialization ${lemon_serialization_SRC})
add_library(lemon::serialization ALIAS lemon_serialization)

target_include_directories(lemon_serialization PUBLIC include/)
target_link_libraries(lemon_serialization
	PUBLIC
	lemon::engine
	PRIVATE
	rapidjson
)