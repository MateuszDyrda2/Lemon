cmake_minimum_required(VERSION 3.22)

# -------------- Library version ---------------
set(lemon_VERSION_MAJOR 0)
set(lemon_VERSION_MINOR 1)
set(lemon_VERSION_PATCH 0)
set(lemon_VERSION ${lemon_VERSION_MAJOR}.${lemon_VERSION_MINOR}.${lemon_VERSION_PATCH})
# ----------------------------------------------

# ---------------- Description -----------------
set(lemon_DESC "2D game engine written in C++")

project(Lemon DESCRIPTION ${lemon_DESC} LANGUAGES CXX)
# ----------------------------------------------

# ------------- External libraries -------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)
# ----------------------------------------------

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(LEMON_PROFILE "Profile execution" ON)
# ----------------------------------------------

# ------------------ Python --------------------
find_package(Python COMPONENTS Interpreter)
# ----------------------------------------------
find_package(Threads REQUIRED)
# ----------------------------------------------

# ------------- Platform specific --------------
if (WIN32)
    # platform windows
    set(LEMON_PLATFORM LEMON_WINDOWS)
elseif (APPLE)
    # platform macos
    set(LEMON_PLATFORM LEMON_APPLE)
elseif (UNIX)
    # platform linux
    set(LEMON_PLATFORM LEMON_LINUX)
endif()
# ----------------------------------------------

# --------------- Add library ------------------
set(lemon_ASSETS_SRC
    src/assets/asset.cpp
    src/assets/asset_loader.cpp
    src/assets/resource.cpp
)
set(lemon_ENGINE_SRC
    src/engine/engine.cpp 
)
set(lemon_EVENTS_SRC
    src/events/events.cpp
)
set(lemon_PLATFORM_SRC
    src/platform/input.cpp
    src/platform/window.cpp
)
set(lemon_RENDERING_SRC
   src/rendering/basic_renderer.cpp 
   src/rendering/batch_renderer.cpp 
   src/rendering/framebuffer.cpp 
   src/rendering/rendering_context.cpp 
   src/rendering/shader.cpp 
   src/rendering/texture.cpp 
   src/rendering/vertex_array.cpp src/rendering/vertex_buffer.cpp 
   src/rendering/systems/rendering_system.cpp
)
set(lemon_THREADING_SRC
    src/threading/scheduler.cpp
)
set(lemon_WORLD_SRC
    src/world/scene.cpp
    src/world/scene_manager.cpp
    src/world/systems/entity_system.cpp
    src/world/systems/transform_system.cpp
)

set(lemon_SRC
    ${lemon_ASSETS_SRC}
    ${lemon_ENGINE_SRC}
    ${lemon_EVENTS_SRC}
    ${lemon_PLATFORM_SRC}
    ${lemon_RENDERING_SRC}
    ${lemon_THREADING_SRC}
    ${lemon_WORLD_SRC}
)

add_library(lemon ${lemon_SRC})
add_library(lemon::lemon ALIAS lemon)
target_include_directories(lemon PUBLIC include/)
target_link_libraries(lemon
  PUBLIC
    Threads::Threads
    concurrentqueue
    fmt
    glad
    glm::glm
    glfw
    rapidjson
    stb
    EnTT::EnTT
)
# ----------------------------------------------

# -------------- Compile options ---------------
target_compile_features(lemon PUBLIC cxx_std_20)
set_target_properties(lemon PROPERTIES CXX_STANDARD_REQUIRED ON)

if(MSVC)
    target_compile_options(lemon
      PUBLIC
        "/W4" "/Zc:preprocessor" "$<$<CONFIG:Debug>:/O2>"
    )
else()
    target_compile_options(lemon 
      PUBLIC
        "-Wall" "-Wextra" "-Wformat=2" "-Wunused" "-fvisibility=hidden" "-Wno-attributes" "$<$<CONFIG:Debug>:-O3>"
    )
endif(MSVC)

target_compile_definitions(lemon 
  PUBLIC
    "$<$<CONFIG:Debug>:LEMON_DEBUG>"
    "${LEMON_PLATFORM}"
    "$<$<BOOL:${LEMON_PROFILE}>:LEMON_PROFILE>"
)
# ----------------------------------------------

# ------------ Pre-compiled headers ------------
target_precompile_headers(lemon 
  PUBLIC
    [["core/lemon_types.h"]]
    [["core/defines.h"]]
    [["core/assert.h"]]
    [["core/logger.h"]]
    [["core/utils.h"]]
  PRIVATE
    <algorithm>
    <cmath>
    <string>
    <vector>
    <cstdio>
)
# ----------------------------------------------

# --------------- Build step -------------------
set(COMPONENT_DEFINITIONS)
set(SYSTEM_DEFINITIONS)
function(add_components)
    foreach(path IN LISTS ARGN)
        set(filename)
        get_filename_component(filename ${path} NAME_WE)
        set(output "${CMAKE_CURRENT_BINARY_DIR}/_generated/${filename}.json")
        add_custom_command(
            OUTPUT ${output}
            DEPENDS ${path}
            COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/export_components.py "${output}" "${CMAKE_CURRENT_SOURCE_DIR}/${path}"
            COMMENT "Generated editor data for ${filename}"
        )
        set(COMPONENT_DEFINITIONS ${COMPONENT_DEFINITIONS} ${output})
    endforeach(path in LISTS ARGN)
    set(COMPONENT_DEFINITIONS ${COMPONENT_DEFINITIONS} PARENT_SCOPE)
endfunction(add_components)

function(add_systems)
    foreach(path IN LISTS ARGN)
        set(filename)
        get_filename_component(filename ${path} NAME_WE)
        set(output "${CMAKE_CURRENT_BINARY_DIR}/_generated/${filename}.json")
        add_custom_command(
            OUTPUT ${output}
            DEPENDS ${path}
            COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/export_systems.py "${output}" "${CMAKE_CURRENT_SOURCE_DIR}/${path}"
            COMMENT "Generated editor data for ${filename}"
        )
        set(SYSTEM_DEFINITIONS ${SYSTEM_DEFINITIONS} ${output})
    endforeach(path in LISTS ARGN)
    set(SYSTEM_DEFINITIONS ${SYSTEM_DEFINITIONS} PARENT_SCOPE)
endfunction(add_systems)

add_components(
    "include/world/components/entity_components.h"
    "include/world/components/transform_components.h"
    "include/rendering/components/rendering_components.h"
)
add_systems(
    "include/world/systems/entity_system.h"
    "include/world/systems/transform_system.h"
    "include/rendering/systems/rendering_system.h"
)
# ----------------------------------------------


# ---------------- Post build ------------------
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/_generated/types.json
    DEPENDS ${COMPONENT_DEFINITIONS} ${SYSTEM_DEFINITIONS}
    COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/write_component_file.py "${CMAKE_CURRENT_BINARY_DIR}/_generated" "${CMAKE_CURRENT_BINARY_DIR}/_generated/types.json"
    COMMENT "Generated types.json"
)
add_custom_target(lemon_generated ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/_generated/types.json)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_generated)
# ----------------------------------------------


