#include <lemon/engine/py_bindings/py_scene.h>

#include <lemon/engine/systems/physics_system.h>
#include <lemon/scene/basic_components.h>
#include <lemon/scene/entity.h>
#include <lemon/scene/scene_manager.h>

#include <lemon/scripting/py_script.h>

namespace lemon {
void py_init_scene()
{
}
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(scene, m)
{
    py::class_<lemon::scene>(m, "scene")
        .def(py::init<string_id>());
    py::class_<scene_manager>(m, "scene_manager")
        .def("push_scene", &scene_manager::push_scene,
             py::return_value_policy::reference)
        .def("pop_scene", &scene_manager::pop_scene)
        .def("get_current_scene", &scene_manager::get_current_scene,
             py::return_value_policy::reference);
    py::class_<component>(m, "component");
    py::class_<transform, component>(m, "transform")
        .def_readwrite("position", &transform::position)
        .def_readwrite("scale", &transform::scale)
        .def_readwrite("rotation", &transform::rotation);
    py::class_<camera, component>(m, "camera")
        .def_readonly("viewport", &camera::viewport);
    py::class_<sprite_renderer, component>(m, "sprite_renderer")
        .def_readwrite("color", &sprite_renderer::color)
        .def_readwrite("texCoords", &sprite_renderer::texCoords)
        .def_readwrite("texture", &sprite_renderer::text);
    py::class_<audio_source, component>(m, "audio_source")
        .def_readwrite("clip", &audio_source::clip)
        .def_readwrite("pitch", &audio_source::pitch)
        .def_readwrite("gain", &audio_source::gain)
        .def_readwrite("loop", &audio_source::loop);
    py::class_<audio_listener, component>(m, "audio_listener")
        .def_readwrite("masterGain", &audio_listener::masterGain);
    py::class_<entity>(m, "entity");
    py::class_<scriptable_entity>(m, "scriptable_entity")
        .def(py::init<entity>())
        .def_readonly("entity", &scriptable_entity::ent)
        .def_property(
            "transform", [](scriptable_entity& ent) { return ent.ent.get_component<transform>(); },
            [](scriptable_entity& ent) { ent.ent.change_component<transform>(); }, py::return_value_policy::reference)
        .def("translate", [](scriptable_entity& ent, const vec2& by) { ent.ent.patch_component<transform>([by](transform& t) { t.position += by; }); })
        .def("rotate", [](scriptable_entity& ent, f32 by) { ent.ent.patch_component<transform>([by](transform& t) { t.rotation += by; }); })
        .def("scale", [](scriptable_entity& ent, const vec2& by) { ent.ent.patch_component<transform>([by](transform& t) { t.scale += by; }); })
        .def_property_readonly("name", [](const scriptable_entity& e) { return e.get_tag().id.get_string(); })
        .def_property("enabled", &scriptable_entity::get_enabled, &scriptable_entity::set_enabled)
        .def("move_position", [](scriptable_entity& ent, const vec2& pos) { physics_system::move_entity(ent.ent, pos); })
        .def(
            "get_component", [](scriptable_entity& ent, const std::string& str) {
                return reflection::get_registeredComponents()[str].get_component_f(ent.ent);
            },
            py::return_value_policy::reference);
}
} // namespace lemon
