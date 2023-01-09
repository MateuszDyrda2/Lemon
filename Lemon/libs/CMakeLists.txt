
add_subdirectory(concurrentqueue)
add_subdirectory(fmt)
add_subdirectory(glad)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

add_subdirectory(glm)

set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_CXX17 ON CACHE BOOL "" FORCE)
add_subdirectory(rapidjson)
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/rapidjson>"
)

add_subdirectory(stb)

add_subdirectory(entt)

#add_subdirectory(luajit)
add_subdirectory(lua)

add_subdirectory(luabridge3)
