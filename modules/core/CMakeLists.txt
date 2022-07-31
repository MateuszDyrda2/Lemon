project(LemonCore)

add_library(lemon_core INTERFACE)
add_library(lemon::core ALIAS lemon_core)
target_include_directories(lemon_core INTERFACE include/)

add_subdirectory(libs/glm)
add_subdirectory(libs/fmt)
target_link_libraries(lemon_core 
	INTERFACE ${GLM_LIBRARIES}
	INTERFACE fmt::fmt	
)
