#include <lemon/engine/py_bindings/py_scene.h>

#include <lemon/physics/systems/physics_system.h>

#include <lemon/audio/components/audio_components.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/rendering/components/rendering_components.h>
#include <lemon/scene/components/transform_components.h>
#include <lemon/scripting/components/scripting_components.h>

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
        .def_readwrite("color", &sprite_renderer::col)
        .def_readwrite("texCoords", &sprite_renderer::texCoords)
        .def_readwrite("texture", &sprite_renderer::text);
    py::class_<audio_source, component>(m, "audio_source")
        .def_readwrite("clip", &audio_source::clip)
        .def_readwrite("pitch", &audio_source::pitch)
        .def_readwrite("gain", &audio_source::gain)
        .def_readwrite("loop", &audio_source::loop);
    py::class_<audio_listener, component>(m, "audio_listener")
        .def_readwrite("masterGain", &audio_listener::masterGain);
    py::class_<rigidbody, component>(m, "rigidbody")
        .def_readwrite("velocity", &rigidbody::velocity)
        .def_readwrite("angularVelocity", &rigidbody::angularVelocity)
        .def_readwrite("linearDrag", &rigidbody::linearDrag)
        .def_readwrite("angularDrag", &rigidbody::angularDrag)
        .def_readwrite("mass", &rigidbody::mass)
        .def_readwrite("gravityScale", &rigidbody::gravityScale)
        .def_readwrite("freezeRotation", &rigidbody::freezeRotation);
    py::class_<entity>(m, "entity");
    py::class_<scriptable_entity>(m, "scriptable_entity")
        .def(py::init<entity>())
        .def_readonly("entity", &scriptable_entity::ent)
        .def_property_readonly("name", [](const scriptable_entity& e) { return e.get_tag().id.get_string(); })
        .def_property("enabled", &scriptable_entity::get_enabled, &scriptable_entity::set_enabled)
        .def(
            "get_component", [](scriptable_entity& ent, const std::string& str) {
                return reflection::get_registeredComponents()[str].get_component_f(ent.ent);
            },
            py::return_value_policy::reference)
        .def("add_component", [](scriptable_entity& ent, const std::string& str, const component& c) {
            reflection::get_registeredComponents()[str].add_component_f(ent.ent, c);
        })
        .def("has_component", [](scriptable_entity& ent, const std::string& str) {
            return reflection::get_registeredComponents()[str].has_component_f(ent.ent);
        });
}
} // namespace lemon
