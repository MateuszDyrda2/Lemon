project(LemonAssets)

set(lemon_assets_SRC
	src/asset_loader.cpp
	src/asset_storage.cpp
	src/resource.cpp
)	

add_library(lemon_assets ${lemon_assets_SRC})
add_library(lemon::assets ALIAS lemon_assets)

target_include_directories(lemon_assets PUBLIC include/)
target_link_libraries(lemon_assets 
	PUBLIC
	lemon::core
	PRIVATE
	rapidjson
)
