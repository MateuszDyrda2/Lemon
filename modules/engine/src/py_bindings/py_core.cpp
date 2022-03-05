#include <lemon/engine/py_bindings/py_core.h>

#include <pybind11/chrono.h>
#include <pybind11/embed.h>

#include <lemon/core/game.h>
#include <lemon/core/logger.h>
#include <lemon/core/math/color.h>
#include <lemon/core/math/mat4.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec3.h>
#include <lemon/core/math/vec4.h>
#include <lemon/core/string_id.h>
#include <lemon/core/time/clock.h>

#include <lemon/engine/scene/basic_components.h>
#include <lemon/engine/scene/scene_manager.h>
#include <lemon/window/input.h>
#include <lemon/window/window_base.h>

#include <lemon/assets/asset.h>

#include <lemon/rendering/shader.h>
#include <lemon/rendering/texture.h>

#include <lemon/audio/audio_player.h>
#include <lemon/audio/sound.h>

#include <lemon/threading/scheduler.h>

#include <lemon/events/dispatcher.h>
#include <lemon/events/listener.h>

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
                .def_readwrite("y", &vec2::y);
            py::class_<ivec2>(math, "ivec2")
                .def(py::init<>())
                .def(py::init<i32, i32>())
                .def_readwrite("x", &ivec2::x)
                .def_readwrite("y", &ivec2::y);
            py::class_<uvec2>(math, "uvec2")
                .def(py::init<>())
                .def(py::init<u32, u32>())
                .def_readwrite("x", &uvec2::x)
                .def_readwrite("y", &uvec2::y);

            py::class_<vec3>(math, "vec3")
                .def(py::init<>())
                .def(py::init<f32, f32, f32>())
                .def_readwrite("x", &vec3::x)
                .def_readwrite("y", &vec3::y)
                .def_readwrite("z", &vec3::z);
            py::class_<ivec3>(math, "ivec3")
                .def(py::init<>())
                .def(py::init<i32, i32, i32>())
                .def_readwrite("x", &ivec3::x)
                .def_readwrite("y", &ivec3::y)
                .def_readwrite("z", &ivec3::z);
            py::class_<uvec3>(math, "uvec3")
                .def(py::init<>())
                .def(py::init<u32, u32, u32>())
                .def_readwrite("x", &uvec3::x)
                .def_readwrite("y", &uvec3::y)
                .def_readwrite("z", &uvec3::z);

            py::class_<vec4>(math, "vec4")
                .def(py::init<>())
                .def(py::init<f32, f32, f32, f32>())
                .def_readwrite("x", &vec4::x)
                .def_readwrite("y", &vec4::y)
                .def_readwrite("z", &vec4::z)
                .def_readwrite("w", &vec4::w);
            py::class_<ivec4>(math, "ivec4")
                .def(py::init<>())
                .def(py::init<i32, i32, i32, i32>())
                .def_readwrite("x", &ivec4::x)
                .def_readwrite("y", &ivec4::y)
                .def_readwrite("z", &ivec4::z)
                .def_readwrite("w", &ivec4::w);
            py::class_<uvec4>(math, "uvec4")
                .def(py::init<>())
                .def(py::init<u32, u32, u32, u32>())
                .def_readwrite("x", &uvec4::x)
                .def_readwrite("y", &uvec4::y)
                .def_readwrite("z", &uvec4::z)
                .def_readwrite("w", &uvec4::w);
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
                    py::is_operator());
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
    }
    auto window = m.def_submodule("window");
    {
        py::class_<window_base>(window, "window")
            .def_property_readonly("size", &window_base::get_size);
        py::class_<input>(window, "input")
            .def("key_pressed", &input::is_key_pressed)
            .def("mouse_pressed", &input::is_mouse_pressed)
            .def_property_readonly("mousePosition", &input::get_mouse_pos);
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
            .def_static("run", &scheduler::run)
            .def_static("run_after", &scheduler::run_after)
            .def_static("run_at", &scheduler::run_at);
    }
}
} // namespace lemon
