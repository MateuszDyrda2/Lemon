#include <lemon/engine/py_bindings/py_rendering.h>

#include <lemon/rendering/shader.h>
#include <lemon/rendering/texture.h>

#include <pybind11/embed.h>

namespace lemon {
void py_init_rendering()
{
}
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(rendering, m)
{
    py::class_<texture>(m, "texture")
        .def(py::init<string_id, const std::string&>())
        .def(py::init<string_id, const ivec2&, const color&>())
        .def_property_readonly("size", &texture::get_size);
    py::class_<shader>(m, "shader")
        .def(py::init<string_id, const std::string&>());
}
} // namespace lemon
