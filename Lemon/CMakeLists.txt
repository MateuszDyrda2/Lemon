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


find_package(Threads REQUIRED)
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
    src/world/entity.cpp
    src/world/scene.cpp
    src/world/scene_manager.cpp
    src/world/systems/entity_system.cpp
    src/world/systems/transform_system.cpp
    src/world/systems/interpolate_system.cpp
)
set(lemon_PHYSICS_SRC
    src/physics/bvh_tree.cpp
    src/physics/sat_algorithm.cpp
    src/physics/systems/collision_system.cpp
    src/physics/systems/physics_system.cpp
    src/physics/shapes.cpp
    src/physics/detail/collision_set.cpp
)

set(lemon_SRC
    ${lemon_ASSETS_SRC}
    ${lemon_ENGINE_SRC}
    ${lemon_EVENTS_SRC}
    ${lemon_PLATFORM_SRC}
    ${lemon_RENDERING_SRC}
    ${lemon_THREADING_SRC}
    ${lemon_WORLD_SRC}
    ${lemon_PHYSICS_SRC}
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
set(COMPONENTS_LIST
    "include/world/components/entity_components.h"
    "include/world/components/transform_components.h"
    "include/rendering/components/rendering_components.h"
)
set(SYSTEMS_LIST
    "include/world/systems/entity_system.h"
    "include/world/systems/transform_system.h"
    "include/rendering/systems/rendering_system.h"
)

foreach(path IN LISTS COMPONENTS_LIST)
    set(filename)
    get_filename_component(filename ${path} NAME_WE)
    set(output "${CMAKE_BINARY_DIR}/_generated/${filename}.json")
    add_custom_command(
        OUTPUT ${output}
        DEPENDS ${path}
        COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/export_components.py "${output}" "${CMAKE_CURRENT_SOURCE_DIR}/${path}"
        COMMENT "Generated editor data for ${filename}")
    set(ENGINE_TYPES ${ENGINE_TYPES} ${output})
endforeach(path in LISTS ARGN)

foreach(path IN LISTS SYSTEMS_LIST)
    set(filename)
    get_filename_component(filename ${path} NAME_WE)
    set(output "${CMAKE_BINARY_DIR}/_generated/${filename}.json")
    add_custom_command(
        OUTPUT ${output}
        DEPENDS ${path}
        COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/export_systems.py "${output}" "${CMAKE_CURRENT_SOURCE_DIR}/${path}"
        COMMENT "Generated editor data for ${filename}"
    )
    set(ENGINE_TYPES ${ENGINE_TYPES} ${output})
endforeach(path in LISTS ARGN)
set(ENGINE_TYPES ${ENGINE_TYPES} PARENT_SCOPE)

add_custom_target(lemon_engine_types
  ALL DEPENDS
    ${ENGINE_TYPES}
)

# ----------------------------------------------


