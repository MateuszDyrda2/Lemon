#include <lemon/engine/py_bindings/py_platform.h>

#include <lemon/platform/window.h>
#include <lemon/platform/window_events.h>

#include <pybind11/embed.h>

namespace lemon {
void py_init_platform()
{
}
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(platform, m)
{
    py::class_<window>(m, "window")
        .def_property_readonly("size", &window::get_size)
        .def_property_readonly_static("onKeyPressed", [](py::object) { return window::onKeyPressed; })
        .def_property_readonly_static("onMouseButtonPressed", [](py::object) { return window::onMouseButtonPressed; })
        .def_property_readonly_static("onMouseScroll", [](py::object) { return window::onMouseScroll; })
        .def_property_readonly_static("onWindowClose", [](py::object) { return window::onWindowClose; })
        .def_property_readonly_static("onWindowSize", [](py::object) { return window::onWindowSize; })
        .def_property_readonly_static("onFramebufferSize", [](py::object) { return window::onFramebufferSize; })
        .def_property_readonly_static("onWindowContentScale", [](py::object) { return window::onWindowContentScale; })
        .def_property_readonly_static("onWindowPos", [](py::object) { return window::onWindowPos; })
        .def_property_readonly_static("onWindowIconify", [](py::object) { return window::onWindowIconify; })
        .def_property_readonly_static("onWindowMaximize", [](py::object) { return window::onWindowMaximize; })
        .def_property_readonly_static("onWindowFocused", [](py::object) { return window::onWindowFocused; })
        .def_property_readonly_static("onWindowRefresh", [](py::object) { return window::onWindowRefresh; });

    py::class_<KeyPressed, event_args>(m, "KeyPressed")
        .def(py::init<key::keycode, int, key::action, key::keymod>())
        .def_readwrite("keycode", &KeyPressed::keycode)
        .def_readwrite("scancode", &KeyPressed::scancode)
        .def_readwrite("action", &KeyPressed::action)
        .def_readwrite("keymod", &KeyPressed::keymod);
    py::class_<MouseButtonPressed, event_args>(m, "MouseButtonPressed")
        .def(py::init<key::mouse, key::action, key::keymod>())
        .def_readwrite("button", &MouseButtonPressed::button)
        .def_readwrite("action", &MouseButtonPressed::action)
        .def_readwrite("keymod", &MouseButtonPressed::keymod);
    py::class_<MouseScroll, event_args>(m, "MouseScroll")
        .def(py::init<f64, f64>())
        .def_readwrite("xoffset", &MouseScroll::xoffset)
        .def_readwrite("yoffset", &MouseScroll::yoffset);
    py::class_<WindowClose, event_args>(m, "WindowClose")
        .def(py::init<>());
    py::class_<WindowSize, event_args>(m, "WindowSize")
        .def(py::init<i32, i32>())
        .def_readwrite("width", &WindowSize::width)
        .def_readwrite("height", &WindowSize::height);
    py::class_<FramebufferSize, event_args>(m, "FramebufferSize")
        .def(py::init<i32, i32>())
        .def_readwrite("width", &FramebufferSize::width)
        .def_readwrite("height", &FramebufferSize::height);
    py::class_<WindowContentScale, event_args>(m, "WindowContentScale")
        .def(py::init<f32, f32>())
        .def_readwrite("xscale", &WindowContentScale::xscale)
        .def_readwrite("yscale", &WindowContentScale::yscale);
    py::class_<WindowPos, event_args>(m, "WindowPos")
        .def(py::init<i32, i32>())
        .def_readwrite("xpos", &WindowPos::xpos)
        .def_readwrite("ypos", &WindowPos::ypos);
    py::class_<WindowIconify, event_args>(m, "WindowIconify")
        .def(py::init<i32>())
        .def_readwrite("iconified", &WindowIconify::iconified);
    py::class_<WindowMaximize, event_args>(m, "WindowMaximize")
        .def(py::init<i32>())
        .def_readwrite("maximized", &WindowMaximize::maximized);
    py::class_<WindowFocused, event_args>(m, "WindowFocused")
        .def(py::init<i32>())
        .def_readwrite("focused", &WindowFocused::focused);
    py::class_<WindowRefresh, event_args>(m, "WindowRefresh")
        .def(py::init<>());
}
} // namespace lemon
