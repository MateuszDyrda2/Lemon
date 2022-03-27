project(imgui)

set(imgui_SRC
	imgui.cpp
	imgui_draw.cpp
	imgui_impl_glfw.cpp
	imgui_impl_opengl3.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
)
add_library(imgui ${imgui_SRC})
target_include_directories(imgui PUBLIC .)