#include <lemon/engine/py_bindings/py_systems.h>

#include <lemon/engine/systems/physics_system.h>
#include <lemon/scene/components/physics_components.h>

#include <lemon/engine/systems/transform_system.h>
#include <lemon/scene/components/transform_components.h>

#include <lemon/core/math/vec2.h>
#include <lemon/scene/entity.h>
#include <lemon/scripting/py_script.h>

#include <functional>

namespace lemon {
void py_init_systems() { }

namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(physics, m)
{
    py::class_<physics_system>(m, "Physics")
        .def_static("add_force", [](scriptable_entity& ent, const vec2& amount) {
            physics_system::add_force(ent.ent, amount);
        })
        .def_static("move_position", [](scriptable_entity& ent, const vec2& to) {
            physics_system::move_entity(ent.ent, to);
        })
        .def_static("move_rotation", [](scriptable_entity& ent, f32 to) {
            physics_system::rotate_entity(ent.ent, to);
        })
        .def_static("add_impulse", [](scriptable_entity& ent, const vec2& amount) {
            physics_system::add_impulse(ent.ent, amount);
        })
        .def_static("add_torque", [](scriptable_entity& ent, f32 amount) {
            physics_system::add_torque(ent.ent, amount);
        });
}
PYBIND11_EMBEDDED_MODULE(transform, m)
{
    py::class_<transform_system>(m, "Transform")
        .def_static("translate", [](scriptable_entity& ent, const vec2& by) {
            transform_system::translate(ent.ent, by);
        })
        .def_static("rotate", [](scriptable_entity& ent, f32 by) {
            transform_system::rotate(ent.ent, by);
        })
        .def_static("scale", [](scriptable_entity& ent, const vec2& by) {
            transform_system::scale(ent.ent, by);
        });
}
}