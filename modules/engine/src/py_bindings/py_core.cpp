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

#include <lemon/scene/basic_components.h>
#include <lemon/scene/entity.h>
#include <lemon/scene/scene_manager.h>
#include <lemon/window/input.h>
#include <lemon/window/window_base.h>
#include <lemon/window/window_events.h>

#include <lemon/assets/asset.h>

#include <lemon/rendering/shader.h>
#include <lemon/rendering/texture.h>

#include <lemon/audio/audio_player.h>
#include <lemon/audio/sound.h>

#include <lemon/threading/scheduler.h>

#include <lemon/events/event_handler.h>

namespace lemon {
void init_py()
{
    LOG_WARN("py init");
}
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(lemon, m)
{
    auto core = m.def_submodule("core");
    {
        // ------  LOGGER -----------
        py::class_<logger>(core, "logger")
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
        py::class_<string_id>(core, "string_id")
            .def(py::init<const char*>())
            .def_property("str", &string_id::get_string, nullptr)
            .def_property("id", &string_id::get_id, nullptr);
        // --------------------------
        // -------- CLOCK -----------
        auto time = core.def_submodule("time");
        {
            py::class_<clock>(time, "clock")
                .def(py::init<>());
        }
        // --------------------------
        // -------- GAME ------------
        py::class_<game>(core, "game")
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
                py::return_value_policy::reference)
            .def_static(
                "input",
                static_cast<ptr<input> (*)()>(&game::get_input_handler),
                py::return_value_policy::reference);
        // --------------------------
        // ---------- MATH ----------
        auto math = core.def_submodule("math");
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
    auto scene = m.def_submodule("scene");
    {
        py::class_<lemon::scene>(scene, "scene")
            .def(py::init<string_id>());
        py::class_<scene_manager>(scene, "scene_manager")
            .def("push_scene", &scene_manager::push_scene,
                 py::return_value_policy::reference)
            .def("pop_scene", &scene_manager::pop_scene)
            .def("get_current_scene", &scene_manager::get_current_scene,
                 py::return_value_policy::reference);
        py::class_<transform>(scene, "transform")
            .def_readwrite("position", &transform::position)
            .def_readwrite("scale", &transform::scale)
            .def_readwrite("rotation", &transform::rotation);
        py::class_<camera>(scene, "camera")
            .def_readonly("viewport", &camera::viewport);
        py::class_<sprite_renderer>(scene, "sprite_renderer")
            .def_readwrite("color", &sprite_renderer::color)
            .def_readwrite("texCoords", &sprite_renderer::texCoords)
            .def_readwrite("texture", &sprite_renderer::text);
        py::class_<audio_source>(scene, "audio_source")
            .def_readwrite("clip", &audio_source::clip)
            .def_readwrite("pitch", &audio_source::pitch)
            .def_readwrite("gain", &audio_source::gain)
            .def_readwrite("loop", &audio_source::loop);
        py::class_<audio_listener>(scene, "audio_listener")
            .def_readwrite("masterGain", &audio_listener::masterGain);
        py::class_<entity>(scene, "entity")
            .def_property_readonly("id", [](const entity& ent) {
                return ent.get_component<tag>().id.get_id();
            })
            .def_property_readonly("handle", &entity::get_handle)
            .def_property("transform", py::overload_cast<>(&entity::get_component<transform>, py::const_), [](entity& ent) {
                ent.emplace_or_replace<dirty>();
                return ent.get_component<transform>();
            })
            .def("translate", [](entity& ent, const vec3& by) {
                ent.get_component<transform>().position += by;
                ent.emplace_or_replace<dirty>();
            });
    }
    auto events = m.def_submodule("events");
    {
        py::class_<event_base>(events, "event")
            .def(py::init<>());
        py::class_<event_handler>(events, "handler")
            .def_static("fire",
                        py::overload_cast<
                            const std::string&, event_base*>(
                            &event_handler::fire_event))
            .def_static("subscribe",
                        py::overload_cast<
                            const std::string&, const std::function<void(event_base*)>&>(
                            &event_handler::subscribe));
        events.def("on_event", [](const std::string& name) {
            std::function<std::function<void(event_base*)>(const std::function<void(event_base*)>&)> f =
                [name](const std::function<void(event_base*)>& fun) {
                    event_handler::subscribe(name, fun);
                    return fun;
                };
            return f;
        });
    }
    auto window = m.def_submodule("window");
    {
        py::class_<window_base>(window, "window")
            .def_property_readonly("size", &window_base::get_size);
        py::class_<input>(window, "input")
            .def("key_pressed", &input::is_key_pressed)
            .def("mouse_pressed", &input::is_mouse_pressed)
            .def_property_readonly("mousePosition", &input::get_mouse_pos);
        auto key = window.def_submodule("key");
        {
            py::enum_<key::action>(key, "action")
                .value("up", key::action::up)
                .value("down", key::action::down)
                .value("repeat", key::action::repeat)
                .export_values();
            py::enum_<key::keycode>(key, "keycode")
                .value("space", key::keycode::space)
                .value("apostrophe", key::keycode::apostrophe)
                .value("comma", key::keycode::comma)
                .value("minus", key::keycode::minus)
                .value("period", key::keycode::period)
                .value("slash", key::keycode::slash)
                .value("key_0", key::keycode::key_0)
                .value("key_1", key::keycode::key_1)
                .value("key_2", key::keycode::key_2)
                .value("key_3", key::keycode::key_3)
                .value("key_4", key::keycode::key_4)
                .value("key_5", key::keycode::key_5)
                .value("key_6", key::keycode::key_6)
                .value("key_7", key::keycode::key_7)
                .value("key_8", key::keycode::key_8)
                .value("key_9", key::keycode::key_9)
                .value("semicolon", key::keycode::semicolon)
                .value("equal", key::keycode::equal)
                .value("a", key::keycode::a)
                .value("b", key::keycode::b)
                .value("c", key::keycode::c)
                .value("d", key::keycode::d)
                .value("e", key::keycode::e)
                .value("f", key::keycode::f)
                .value("g", key::keycode::g)
                .value("h", key::keycode::h)
                .value("i", key::keycode::i)
                .value("j", key::keycode::j)
                .value("k", key::keycode::k)
                .value("l", key::keycode::l)
                .value("m", key::keycode::m)
                .value("n", key::keycode::n)
                .value("o", key::keycode::o)
                .value("p", key::keycode::p)
                .value("q", key::keycode::q)
                .value("r", key::keycode::r)
                .value("s", key::keycode::s)
                .value("t", key::keycode::t)
                .value("u", key::keycode::u)
                .value("v", key::keycode::v)
                .value("w", key::keycode::w)
                .value("x", key::keycode::x)
                .value("y", key::keycode::y)
                .value("z", key::keycode::z)
                .value("left_bracket", key::keycode::left_bracket)
                .value("backslash", key::keycode::backslash)
                .value("right_bracket", key::keycode::right_bracket)
                .value("grave_accent", key::keycode::grave_accent)
                .value("world_1", key::keycode::world_1)
                .value("world_2", key::keycode::world_2)
                .value("escape", key::keycode::escape)
                .value("enter", key::keycode::enter)
                .value("tab", key::keycode::tab)
                .value("backspace", key::keycode::backspace)
                .value("insert", key::keycode::insert)
                .value("del", key::keycode::del)
                .value("right_arrow", key::keycode::right_arrow)
                .value("left_arrow", key::keycode::left_arrow)
                .value("down_arrow", key::keycode::down_arrow)
                .value("up_arrow", key::keycode::up_arrow)
                .value("page_up", key::keycode::page_up)
                .value("page_down", key::keycode::page_down)
                .value("home", key::keycode::home)
                .value("end", key::keycode::end)
                .value("caps_lock", key::keycode::caps_lock)
                .value("scroll_lock", key::keycode::scroll_lock)
                .value("num_lock", key::keycode::num_lock)
                .value("print_screen", key::keycode::print_screen)
                .value("pause", key::keycode::pause)
                .value("f1", key::keycode::f1)
                .value("f2", key::keycode::f2)
                .value("f3", key::keycode::f3)
                .value("f4", key::keycode::f4)
                .value("f5", key::keycode::f5)
                .value("f6", key::keycode::f6)
                .value("f7", key::keycode::f7)
                .value("f8", key::keycode::f8)
                .value("f9", key::keycode::f9)
                .value("f10", key::keycode::f10)
                .value("f11", key::keycode::f11)
                .value("f12", key::keycode::f12)
                .value("f13", key::keycode::f13)
                .value("f14", key::keycode::f14)
                .value("f15", key::keycode::f15)
                .value("f16", key::keycode::f16)
                .value("f17", key::keycode::f17)
                .value("f18", key::keycode::f18)
                .value("f19", key::keycode::f19)
                .value("f20", key::keycode::f20)
                .value("f21", key::keycode::f21)
                .value("f22", key::keycode::f22)
                .value("f23", key::keycode::f23)
                .value("f24", key::keycode::f24)
                .value("f25", key::keycode::f25)
                .value("kp_0", key::keycode::kp_0)
                .value("kp_1", key::keycode::kp_1)
                .value("kp_2", key::keycode::kp_2)
                .value("kp_3", key::keycode::kp_3)
                .value("kp_4", key::keycode::kp_4)
                .value("kp_5", key::keycode::kp_5)
                .value("kp_6", key::keycode::kp_6)
                .value("kp_7", key::keycode::kp_7)
                .value("kp_8", key::keycode::kp_8)
                .value("kp_9", key::keycode::kp_9)
                .value("kp_decimal", key::keycode::kp_decimal)
                .value("kp_divide", key::keycode::kp_divide)
                .value("kp_multiply", key::keycode::kp_multiply)
                .value("kp_subtract", key::keycode::kp_subtract)
                .value("kp_add", key::keycode::kp_add)
                .value("kp_enter", key::keycode::kp_enter)
                .value("kp_equal", key::keycode::kp_equal)
                .value("left_shift", key::keycode::left_shift)
                .value("left_control", key::keycode::left_control)
                .value("left_alt", key::keycode::left_alt)
                .value("left_super", key::keycode::left_super)
                .value("right_shift", key::keycode::right_shift)
                .value("right_control", key::keycode::right_control)
                .value("right_alt", key::keycode::right_alt)
                .value("right_super", key::keycode::right_super)
                .value("menu", key::keycode::menu)
                .export_values();
            py::enum_<key::keymod>(key, "keymod")
                .value("SHIFT", key::keymod::SHIFT)
                .value("CONTROL", key::keymod::CONTROL)
                .value("ALT", key::keymod::ALT)
                .value("SUPER", key::keymod::SUPER)
                .value("CAPS_LOCK", key::keymod::CAPS_LOCK)
                .value("NUM_LOCK", key::keymod::NUM_LOCK)
                .export_values();
            py::enum_<key::mouse>(key, "mouse")
                .value("button_1", key::mouse::button_1)
                .value("button_2", key::mouse::button_2)
                .value("button_3", key::mouse::button_3)
                .value("button_4", key::mouse::button_4)
                .value("button_5", key::mouse::button_5)
                .value("button_6", key::mouse::button_6)
                .value("button_7", key::mouse::button_7)
                .value("button_8", key::mouse::button_8)
                .export_values();
        }

        py::class_<KeyPressed, event_base>(window, "KeyPressed")
            .def(py::init<key::keycode, int, key::action, key::keymod>())
            .def_readwrite("keycode", &KeyPressed::keycode)
            .def_readwrite("scancode", &KeyPressed::scancode)
            .def_readwrite("action", &KeyPressed::action)
            .def_readwrite("keymod", &KeyPressed::keymod);

        py::class_<MouseButtonPressed, event_base>(window, "MouseButtonPressed")
            .def(py::init<key::mouse, key::action, key::keymod>())
            .def_readwrite("button", &MouseButtonPressed::button)
            .def_readwrite("action", &MouseButtonPressed::action)
            .def_readwrite("keymod", &MouseButtonPressed::keymod);

        py::class_<MouseScroll, event_base>(window, "MouseScroll")
            .def(py::init<f64, f64>())
            .def_readwrite("xoffset", &MouseScroll::xoffset)
            .def_readwrite("yoffset", &MouseScroll::yoffset);
        py::class_<WindowClose, event_base>(window, "WindowClose")
            .def(py::init<>());
        py::class_<WindowSize, event_base>(window, "WindowSize")
            .def(py::init<i32, i32>())
            .def_readwrite("width", &WindowSize::width)
            .def_readwrite("height", &WindowSize::height);
        py::class_<FramebufferSize, event_base>(window, "FramebufferSize")
            .def(py::init<i32, i32>())
            .def_readwrite("width", &FramebufferSize::width)
            .def_readwrite("height", &FramebufferSize::height);
        py::class_<WindowContentScale, event_base>(window, "WindowContentScale")
            .def(py::init<f32, f32>())
            .def_readwrite("xscale", &WindowContentScale::xscale)
            .def_readwrite("yscale", &WindowContentScale::yscale);
        py::class_<WindowPos, event_base>(window, "WindowPos")
            .def(py::init<i32, i32>())
            .def_readwrite("xpos", &WindowPos::xpos)
            .def_readwrite("ypos", &WindowPos::ypos);
        py::class_<WindowIconify, event_base>(window, "WindowIconify")
            .def(py::init<i32>())
            .def_readwrite("iconified", &WindowIconify::iconified);
        py::class_<WindowMaximize, event_base>(window, "WindowMaximize")
            .def(py::init<i32>())
            .def_readwrite("maximized", &WindowMaximize::maximized);
        py::class_<WindowFocused, event_base>(window, "WindowFocused")
            .def(py::init<i32>())
            .def_readwrite("focused", &WindowFocused::focused);
        py::class_<WindowRefresh, event_base>(window, "WindowRefresh")
            .def(py::init<>());
    }
    auto assets = m.def_submodule("assets");
    {
        py::class_<asset<texture>>(assets, "texture_asset")
            .def(py::init<string_id>())
            .def("texture", py::overload_cast<>(&asset<texture>::get, py::const_), py::return_value_policy::reference)
            .def_property_readonly("id", &asset<texture>::get_id)
            .def("__repr__", [](const asset<texture>& a) { return a.get_id().get_string(); });
        py::class_<asset<shader>>(assets, "shader_asset")
            .def(py::init<string_id>())
            .def("shader", py::overload_cast<>(&asset<shader>::get, py::const_), py::return_value_policy::reference)
            .def_property_readonly("id", &asset<shader>::get_id)
            .def("__repr__", [](const asset<shader>& a) { return a.get_id().get_string(); });
        py::class_<asset<sound>>(assets, "sound_asset")
            .def(py::init<string_id>())
            .def("sound", py::overload_cast<>(&asset<sound>::get, py::const_), py::return_value_policy::reference)
            .def_property_readonly("id", &asset<sound>::get_id)
            .def("__repr__", [](const asset<sound>& a) { return a.get_id().get_string(); });
    }
    auto audio = m.def_submodule("audio");
    {
        py::class_<audio_player>(audio, "audio_player")
            .def("play", &audio_player::start_playing);
        py::class_<sound>(audio, "sound")
            .def_property_readonly("sampleRate", [](const sound& a) { return a.get_info().sampleRate; })
            .def_property_readonly("bitDepth", [](const sound& a) { return a.get_info().bitDepth; })
            .def_property_readonly("lenght", [](const sound& a) { return a.get_info().lenght; })
            .def_property_readonly("channelCount", [](const sound& a) { return a.get_info().channelCount; });
    }
    auto rendering = m.def_submodule("rendering");
    {
        py::class_<texture>(rendering, "texture")
            .def(py::init<string_id, const std::string&>())
            .def(py::init<string_id, const ivec2&, const color&>())
            .def_property_readonly("size", &texture::get_size);
        py::class_<shader>(rendering, "shader")
            .def(py::init<string_id, const std::string&>());
    }
    auto threading = m.def_submodule("threading");
    {
        py::class_<scheduler>(threading, "scheduler")
            .def_static("run", &scheduler::run, py::call_guard<py::gil_scoped_release>())
            .def_static("run_after", &scheduler::run_after, py::call_guard<py::gil_scoped_release>())
            .def_static("run_at", &scheduler::run_at, py::call_guard<py::gil_scoped_release>());
    }
}
} // namespace lemon
