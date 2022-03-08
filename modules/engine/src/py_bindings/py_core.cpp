#include <lemon/engine/py_bindings/py_core.h>

#include <pybind11/chrono.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <glm/gtx/string_cast.hpp>
#include <lemon/core/game.h>
#include <lemon/core/logger.h>
#include <lemon/core/math/color.h>
#include <lemon/core/math/mat4.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec3.h>
#include <lemon/core/math/vec4.h>
#include <lemon/core/string_id.h>
#include <lemon/core/time/clock.h>

#include <lemon/engine/py_bindings/py_assets.h>
#include <lemon/engine/py_bindings/py_audio.h>
#include <lemon/engine/py_bindings/py_events.h>
#include <lemon/engine/py_bindings/py_input.h>
#include <lemon/engine/py_bindings/py_rendering.h>
#include <lemon/engine/py_bindings/py_scene.h>
#include <lemon/engine/py_bindings/py_threading.h>
#include <lemon/engine/py_bindings/py_window.h>

#include <lemon/scene/scene_manager.h>
#include <lemon/window/window_base.h>

namespace lemon {
void py_init_core()
{
    LOG_WARN("py init");
    py_init_assets();
    py_init_audio();
    py_init_events();
    py_init_input();
    py_init_scene();
    py_init_threading();
    py_init_window();
    py_init_rendering();
}
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(core, m)
{
    // ------  LOGGER -----------
    py::class_<logger>(m, "logger")
        .def_static(
            "message", static_cast<void (*)(std::string)>(&logger::pprint))
        .def_static(
            "warn", static_cast<void (*)(std::string)>(&logger::pwarn))
        .def_static(
            "error", static_cast<void (*)(std::string)>(&logger::perror))
        .def_static(
            "fatal", static_cast<void (*)(std::string)>(&logger::pfatal));
    // --------------------------
    // ------  STRING_ID --------
    py::class_<string_id>(m, "string_id")
        .def(py::init<const char*>())
        .def_property_readonly("str", &string_id::get_string)
        .def_property_readonly("id", &string_id::get_id)
        .def("__repr__", &string_id::get_string);
    // --------------------------
    // -------- CLOCK -----------
    auto time = m.def_submodule("time");
    {
        py::class_<clock>(time, "clock")
            .def(py::init<>());
    }
    // --------------------------
    // -------- GAME ------------
    py::class_<game>(m, "game")
        .def_static(
            "main_window",
            static_cast<ptr<window_base> (*)()>(&game::get_main_window),
            py::return_value_policy::reference)
        .def_static(
            "game_clock",
            static_cast<ptr<clock> (*)()>(&game::get_game_clock),
            py::return_value_policy::reference)
        .def_static(
            "scene_manager",
            static_cast<ptr<scene_manager> (*)()>(&game::get_scene_manager),
            py::return_value_policy::reference);
    // --------------------------
    // ---------- MATH ----------
    auto math = m.def_submodule("math");
    {
        py::class_<color>(math, "color")
            .def(py::init<>())
            .def(py::init<const vec4&>())
            .def(py::init<f32, f32, f32, f32>())
            .def(py::init<u8, u8, u8, u8>())
            .def(py::init<const char*>())
            .def_readwrite("r", &color::r)
            .def_readwrite("g", &color::g)
            .def_readwrite("b", &color::b)
            .def_readwrite("a", &color::a)
            .def_readwrite("rgba", &color::rgba)
            .def_property_readonly_static("Red", &color::red)
            .def_property_readonly_static("Green", &color::green)
            .def_property_readonly_static("Blue", &color::blue)
            .def_property_readonly_static("White", &color::white)
            .def_property_readonly_static("Black", &color::black)
            .def("__repr__", [](const color& c) { return c.to_string(); });

        py::class_<vec2>(math, "vec2")
            .def(py::init<>())
            .def(py::init<f32, f32>())
            .def_readwrite("x", &vec2::x)
            .def_readwrite("y", &vec2::y)
            .def("lenght", [](const vec2& self) { return glm::length(self); })
            .def_static("dot", [](const vec2& lhs, const vec2& rhs) { return glm::dot(lhs, rhs); })
            .def("normalize", [](vec2& self) { self = glm::normalize(self); })
            .def_static("normalized", [](const vec2& v) { return glm::normalize(v); })
            .def("__eq__", [](const vec2& self, const vec2& rhs) { return self == rhs; })
            .def("__add__", [](vec2& self, const vec2& rhs) { return self + rhs; })
            .def("__iadd__", [](vec2& self, const vec2& other) { self += other; return self; })
            .def("__mul__", [](vec2& self, f32 other) { return self * other; })
            .def("__imul__", [](vec2& self, f32 other) { self *= other; return self; })
            .def("__rmul__", [](vec2& self, f32 other) { return self * other; })
            .def("__sub__", [](vec2& self, const vec2& other) { return self - other; })
            .def("__isub__", [](vec2& self, const vec2& other) {  self -= other; return self; })
            .def("__neg__", [](vec2& self) { return -self; })
            .def("__copy__", [](vec2& self) { return self; })
            .def("copy", [](vec2& self) { return self; })
            .def("__repr__", [](vec2& self) { return glm::to_string(self); });

        py::class_<ivec2>(math, "ivec2")
            .def(py::init<>())
            .def(py::init<i32, i32>())
            .def_readwrite("x", &ivec2::x)
            .def_readwrite("y", &ivec2::y)
            .def("__eq__", [](const ivec2& self, const ivec2& rhs) { return self == rhs; })
            .def("__add__", [](ivec2& self, const ivec2& rhs) { return self + rhs; })
            .def("__iadd__", [](ivec2& self, const ivec2& other) { self += other; return self; })
            .def("__mul__", [](ivec2& self, i32 other) { return self * other; })
            .def("__imul__", [](ivec2& self, i32 other) { self *= other; return self; })
            .def("__rmul__", [](ivec2& self, i32 other) { return self * other; })
            .def("__sub__", [](ivec2& self, const ivec2& other) { return self - other; })
            .def("__isub__", [](ivec2& self, const ivec2& other) {  self -= other; return self; })
            .def("__neg__", [](ivec2& self) { return -self; })
            .def("__copy__", [](ivec2& self) { return self; })
            .def("copy", [](ivec2& self) { return self; })
            .def("__repr__", [](ivec2& self) { return glm::to_string(self); });

        py::class_<uvec2>(math, "uvec2")
            .def(py::init<>())
            .def(py::init<u32, u32>())
            .def_readwrite("x", &uvec2::x)
            .def_readwrite("y", &uvec2::y)
            .def("__eq__", [](const uvec2& self, const uvec2& rhs) { return self == rhs; })
            .def("__add__", [](uvec2& self, const uvec2& rhs) { return self + rhs; })
            .def("__iadd__", [](uvec2& self, const uvec2& other) { self += other; return self; })
            .def("__mul__", [](uvec2& self, u32 other) { return self * other; })
            .def("__imul__", [](uvec2& self, u32 other) { self *= other; return self; })
            .def("__rmul__", [](uvec2& self, u32 other) { return self * other; })
            .def("__sub__", [](uvec2& self, const uvec2& other) { return self - other; })
            .def("__isub__", [](uvec2& self, const uvec2& other) {  self -= other; return self; })
            .def("__neg__", [](uvec2& self) { return -self; })
            .def("__copy__", [](uvec2& self) { return self; })
            .def("copy", [](uvec2& self) { return self; })
            .def("__repr__", [](uvec2& self) { return glm::to_string(self); });

        py::class_<vec3>(math, "vec3")
            .def(py::init<>())
            .def(py::init<f32, f32, f32>())
            .def_readwrite("x", &vec3::x)
            .def_readwrite("y", &vec3::y)
            .def_readwrite("z", &vec3::z)
            .def("lenght", [](const vec3& self) { return glm::length(self); })
            .def_static("dot", [](const vec3& lhs, const vec3& rhs) { return glm::dot(lhs, rhs); })
            .def("normalize", [](vec3& self) { self = glm::normalize(self); })
            .def_static("normalized", [](const vec3& v) { return glm::normalize(v); })
            .def("__eq__", [](const vec3& self, const vec3& rhs) { return self == rhs; })
            .def("__add__", [](vec3& self, const vec3& rhs) { return self + rhs; })
            .def("__iadd__", [](vec3& self, const vec3& other) { self += other; return self; })
            .def("__mul__", [](vec3& self, f32 other) { return self * other; })
            .def("__imul__", [](vec3& self, f32 other) { self *= other; return self; })
            .def("__rmul__", [](vec3& self, f32 other) { return self * other; })
            .def("__sub__", [](vec3& self, const vec3& other) { return self - other; })
            .def("__isub__", [](vec3& self, const vec3& other) {  self -= other; return self; })
            .def("__neg__", [](vec3& self) { return -self; })
            .def("__copy__", [](vec3& self) { return self; })
            .def("copy", [](vec3& self) { return self; })
            .def("__repr__", [](vec3& self) { return glm::to_string(self); });

        py::class_<ivec3>(math, "ivec3")
            .def(py::init<>())
            .def(py::init<i32, i32, i32>())
            .def_readwrite("x", &ivec3::x)
            .def_readwrite("y", &ivec3::y)
            .def_readwrite("z", &ivec3::z)
            .def("__eq__", [](const ivec3& self, const ivec3& rhs) { return self == rhs; })
            .def("__add__", [](ivec3& self, const ivec3& rhs) { return self + rhs; })
            .def("__iadd__", [](ivec3& self, const ivec3& other) { self += other; return self; })
            .def("__mul__", [](ivec3& self, i32 other) { return self * other; })
            .def("__imul__", [](ivec3& self, i32 other) { self *= other; return self; })
            .def("__rmul__", [](ivec3& self, i32 other) { return self * other; })
            .def("__sub__", [](ivec3& self, const ivec3& other) { return self - other; })
            .def("__isub__", [](ivec3& self, const ivec3& other) {  self -= other; return self; })
            .def("__neg__", [](ivec3& self) { return -self; })
            .def("__copy__", [](ivec3& self) { return self; })
            .def("copy", [](ivec3& self) { return self; })
            .def("__repr__", [](ivec3& self) { return glm::to_string(self); });

        py::class_<uvec3>(math, "uvec3")
            .def(py::init<>())
            .def(py::init<u32, u32, u32>())
            .def_readwrite("x", &uvec3::x)
            .def_readwrite("y", &uvec3::y)
            .def_readwrite("z", &uvec3::z)
            .def("__eq__", [](const uvec3& self, const uvec3& rhs) { return self == rhs; })
            .def("__add__", [](uvec3& self, const uvec3& rhs) { return self + rhs; })
            .def("__iadd__", [](uvec3& self, const uvec3& other) { self += other; return self; })
            .def("__mul__", [](uvec3& self, u32 other) { return self * other; })
            .def("__imul__", [](uvec3& self, u32 other) { self *= other; return self; })
            .def("__rmul__", [](uvec3& self, u32 other) { return self * other; })
            .def("__sub__", [](uvec3& self, const uvec3& other) { return self - other; })
            .def("__isub__", [](uvec3& self, const uvec3& other) {  self -= other; return self; })
            .def("__neg__", [](uvec3& self) { return -self; })
            .def("__copy__", [](uvec3& self) { return self; })
            .def("copy", [](uvec3& self) { return self; })
            .def("__repr__", [](uvec3& self) { return glm::to_string(self); });

        py::class_<vec4>(math, "vec4")
            .def(py::init<>())
            .def(py::init<f32, f32, f32, f32>())
            .def_readwrite("x", &vec4::x)
            .def_readwrite("y", &vec4::y)
            .def_readwrite("z", &vec4::z)
            .def_readwrite("w", &vec4::w)
            .def("lenght", [](const vec4& self) { return glm::length(self); })
            .def_static("dot", [](const vec4& lhs, const vec4& rhs) { return glm::dot(lhs, rhs); })
            .def("normalize", [](vec4& self) { self = glm::normalize(self); })
            .def_static("normalized", [](const vec4& v) { return glm::normalize(v); })
            .def("__eq__", [](const vec4& self, const vec4& rhs) { return self == rhs; })
            .def("__add__", [](vec4& self, const vec4& rhs) { return self + rhs; })
            .def("__iadd__", [](vec4& self, const vec4& other) { self += other; return self; })
            .def("__mul__", [](vec4& self, f32 other) { return self * other; })
            .def("__imul__", [](vec4& self, f32 other) { self *= other; return self; })
            .def("__rmul__", [](vec4& self, f32 other) { return self * other; })
            .def("__sub__", [](vec4& self, const vec4& other) { return self - other; })
            .def("__isub__", [](vec4& self, const vec4& other) {  self -= other; return self; })
            .def("__neg__", [](vec4& self) { return -self; })
            .def("__copy__", [](vec4& self) { return self; })
            .def("copy", [](vec4& self) { return self; })
            .def("__repr__", [](vec4& self) { return glm::to_string(self); });

        py::class_<ivec4>(math, "ivec4")
            .def(py::init<>())
            .def(py::init<i32, i32, i32, i32>())
            .def_readwrite("x", &ivec4::x)
            .def_readwrite("y", &ivec4::y)
            .def_readwrite("z", &ivec4::z)
            .def_readwrite("w", &ivec4::w)
            .def("__eq__", [](const ivec4& self, const ivec4& rhs) { return self == rhs; })
            .def("__add__", [](ivec4& self, const ivec4& rhs) { return self + rhs; })
            .def("__iadd__", [](ivec4& self, const ivec4& other) { self += other; return self; })
            .def("__mul__", [](ivec4& self, i32 other) { return self * other; })
            .def("__imul__", [](ivec4& self, i32 other) { self *= other; return self; })
            .def("__rmul__", [](ivec4& self, i32 other) { return self * other; })
            .def("__sub__", [](ivec4& self, const ivec4& other) { return self - other; })
            .def("__isub__", [](ivec4& self, const ivec4& other) {  self -= other; return self; })
            .def("__neg__", [](ivec4& self) { return -self; })
            .def("__copy__", [](ivec4& self) { return self; })
            .def("copy", [](ivec4& self) { return self; })
            .def("__repr__", [](ivec4& self) { return glm::to_string(self); });

        py::class_<uvec4>(math, "uvec4")
            .def(py::init<>())
            .def(py::init<u32, u32, u32, u32>())
            .def_readwrite("x", &uvec4::x)
            .def_readwrite("y", &uvec4::y)
            .def_readwrite("z", &uvec4::z)
            .def_readwrite("w", &uvec4::w)
            .def("__eq__", [](const uvec4& self, const uvec4& rhs) { return self == rhs; })
            .def("__add__", [](uvec4& self, const uvec4& rhs) { return self + rhs; })
            .def("__iadd__", [](uvec4& self, const uvec4& other) { self += other; return self; })
            .def("__mul__", [](uvec4& self, u32 other) { return self * other; })
            .def("__imul__", [](uvec4& self, u32 other) { self *= other; return self; })
            .def("__rmul__", [](uvec4& self, u32 other) { return self * other; })
            .def("__sub__", [](uvec4& self, const uvec4& other) { return self - other; })
            .def("__isub__", [](uvec4& self, const uvec4& other) {  self -= other; return self; })
            .def("__neg__", [](uvec4& self) { return -self; })
            .def("__copy__", [](uvec4& self) { return self; })
            .def("copy", [](uvec4& self) { return self; })
            .def("__repr__", [](uvec4& self) { return glm::to_string(self); });

        py::class_<mat4>(math, "mat4")
            .def(py::init<>())
            .def(py::init<f32>())
            .def(py::init<f32, f32, f32, f32,
                          f32, f32, f32, f32,
                          f32, f32, f32, f32,
                          f32, f32, f32, f32>())
            .def(
                "__getitem__",
                [](const mat4& m, int i) -> const vec4& {
                    return m[i];
                },
                py::is_operator())
            .def(
                "__setitem__",
                [](mat4& m, int i) -> vec4& {
                    return m[i];
                },
                py::is_operator())
            .def("transpose", [](mat4& self) { return glm::transpose(self); })
            .def("inverse", [](mat4& self) { return glm::inverse(self); })
            .def("__mul__", [](mat4& self, mat4& other) { return self * other; })
            .def("__add__", [](mat4& self, mat4& other) { return self + other; })
            .def("__iadd__", [](mat4& self, mat4& other) {  self += other; return self; })
            .def("__sub__", [](mat4& self, mat4& other) { return self - other; })
            .def("__isub__", [](mat4& self, mat4& other) {  self -= other; return self; })
            .def("__copy__", [](mat4& self) { return self; })
            .def("copy", [](mat4& self) { return self; })
            .def("__repr__", [](mat4& self) { return self; });
    }
}
} // namespace lemon
