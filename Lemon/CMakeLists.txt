cmake_minimum_required(VERSION 3.22)

# -------------- Library version ---------------
set(lemon_VERSION_MAJOR 0)
set(lemon_VERSION_MINOR 1)
set(lemon_VERSION_PATCH 0)
set(lemon_VERSION ${lemon_VERSION_MAJOR}.${lemon_VERSION_MINOR}.${lemon_VERSION_PATCH})
# ----------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# ---------------- Description -----------------
set(lemon_DESC "2D game engine written in C++")

project(Lemon DESCRIPTION ${lemon_DESC} LANGUAGES CXX)
# ----------------------------------------------

# ------------- External libraries -------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)
# ----------------------------------------------
find_package(Threads REQUIRED)
# find_package(LuaJIT REQUIRED)
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
   src/rendering/vertex_array.cpp 
   src/rendering/vertex_buffer.cpp 
   src/rendering/animation_script.cpp
   src/rendering/systems/rendering_system.cpp
   src/rendering/systems/animation_system.cpp
)
set(lemon_THREADING_SRC
    src/threading/scheduler.cpp
)
set(lemon_WORLD_SRC
    src/world/entity.cpp
    src/world/scene.cpp
    src/world/scene_manager.cpp
    src/world/entity_registry.cpp
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
set (lemon_SCRIPTING_SRC
    src/scripting/scripting_engine.cpp
    src/scripting/systems/scripting_system.cpp
    src/scripting/script.cpp
    src/scripting/message_bus.cpp
    src/scripting/script_entity.cpp
)

set(lemon_SERIALIZATION_SRC
    src/serialization/basic_types_serializer.cpp
    #    src/serialization/scene_serializer.cpp
    src/serialization/serializer.cpp
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
    ${lemon_SCRIPTING_SRC}
    ${lemon_SERIALIZATION_SRC}
)

add_library(lemon ${lemon_SRC})
add_library(lemon::lemon ALIAS lemon)
target_include_directories(lemon 
  PUBLIC 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/lemon>"
)

set(LINUX_INC )

if(!MSVC)
   set(LINUX_INC Threads::Threads)
endif()

target_link_libraries(lemon
  PUBLIC
    ${LINUX_INC}
    concurrentqueue
    fmt
    glad
    glm::glm
    glfw
    rapidjson
    stb
    EnTT::EnTT
    lua::lua
    LuaBridge
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
set(COMPONENTS_LIST_LEMON
    "${CMAKE_CURRENT_SOURCE_DIR}/include/world/components/entity_components.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/world/components/transform_components.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/rendering/components/rendering_components.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/scripting/components/scripting_components.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/physics/components/physics_components.h"
    PARENT_SCOPE
)
include(Macros)

set(serialization_files ${CMAKE_BINARY_DIR}/_generated/types.cpp PARENT_SCOPE)

#include(GNUInstallDirs)
#install(TARGETS lemon fmt glad glm glfw rapidjson stb lua LuaBridge
#    EXPORT lemonTargets
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)
#install(DIRECTORY include/ 
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lemon
#)
#
#install(EXPORT lemonTargets
#    FILE lemonTargets.cmake
#    NAMESPACE lemon::
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lemon
#)
#
#include(CMakePackageConfigHelpers)
#configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
#    "${CMAKE_CURRENT_BINARY_DIR}/lemonConfig.cmake"
#    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lemon
#)
#install(FILES 
#    "${CMAKE_CURRENT_BINARY_DIR}/lemonConfig.cmake"
#    "${CMAKE_CURRENT_BINARY_DIR}/lemonConfigVersion.cmake"
#    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Macros.cmake"
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lemon
#)
#

# ----------------------------------------------

