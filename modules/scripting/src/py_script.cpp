#include <lemon/scripting/py_script.h>

#include <pybind11/embed.h>

namespace lemon {
namespace py = pybind11;
py_script::py_script(string_id name, const std::string& path):
    resource(name)
{
    auto aa = py::module_::import(path.c_str());
}
py_script::~py_script()
{
}
} // namespace lemon
