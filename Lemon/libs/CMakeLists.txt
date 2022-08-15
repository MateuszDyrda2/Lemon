set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/libs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/libs)

add_subdirectory(concurrentqueue)
add_subdirectory(fmt)
add_subdirectory(glad)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

add_subdirectory(glm)

set(RAPIDJSON_BUILD_DOC  OFF)
set(RAPIDJSON_BUILD_EXAMPLES OFF)
set(RAPIDJSON_BUILD_TESTS OFF)
set(RAPIDJSON_BUILD_CXX17 ON)
add_subdirectory(rapidjson)

add_subdirectory(stb)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)

add_subdirectory(entt)