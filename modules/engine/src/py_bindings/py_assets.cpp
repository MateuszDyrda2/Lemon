#include <lemon/engine/py_bindings/py_assets.h>

#include <lemon/assets/asset.h>
#include <lemon/audio/sound.h>
#include <lemon/rendering/shader.h>
#include <lemon/rendering/texture.h>

#include <pybind11/embed.h>

namespace lemon {
void py_init_assets()
{
}
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(assets, m)
{
    py::class_<asset<texture>>(m, "texture_asset")
        .def(py::init<string_id>())
        .def("texture", py::overload_cast<>(&asset<texture>::get, py::const_), py::return_value_policy::reference)
        .def_property_readonly("id", &asset<texture>::get_id)
        .def("__repr__", [](const asset<texture>& a) { return a.get_id().get_string(); });
    py::class_<asset<shader>>(m, "shader_asset")
        .def(py::init<string_id>())
        .def("shader", py::overload_cast<>(&asset<shader>::get, py::const_), py::return_value_policy::reference)
        .def_property_readonly("id", &asset<shader>::get_id)
        .def("__repr__", [](const asset<shader>& a) { return a.get_id().get_string(); });
    py::class_<asset<sound>>(m, "sound_asset")
        .def(py::init<string_id>())
        .def("sound", py::overload_cast<>(&asset<sound>::get, py::const_), py::return_value_policy::reference)
        .def_property_readonly("id", &asset<sound>::get_id)
        .def("__repr__", [](const asset<sound>& a) { return a.get_id().get_string(); });
}
} // namespace lemon
