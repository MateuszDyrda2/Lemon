project(LemonAssets)

set(lemon_assets_SRC
	src/asset_loader.cpp
	src/asset.cpp
	src/resource.cpp
)	

add_library(lemon_assets ${lemon_assets_SRC})
add_library(lemon::assets ALIAS lemon_assets)
target_include_directories(lemon_assets PUBLIC include/)

set(RAPIDJSON_BUILD_DOC OFF CACHE INTERNAL "do not build docs" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE INTERNAL "do not build examples" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE INTERNAL "do not build tests" FORCE)

add_subdirectory(libs/rapidjson)
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE libs/rapidjson/include)
target_link_libraries(lemon_assets 
	PUBLIC lemon::core
	PRIVATE rapidjson
)
