cmake_minimum_required(VERSION 3.20)

project(imgui)

set(imgui_SRC 
${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_demo.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_draw.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_impl_glfw.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_impl_opengl3.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_tables.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_widgets.cpp
)
add_library(imgui ${imgui_SRC})
target_include_directories(imgui PUBLIC .)