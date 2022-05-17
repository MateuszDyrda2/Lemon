#include <lemon/scripting/py_script.h>

#include <lemon/core/assert.h>

namespace lemon {
py_script::py_script(const std::string& moduleName)
{
    mod = py::module_::import(moduleName.c_str());
    try
    {
        py_class = mod.attr(moduleName.c_str());
    }
    catch(const py::error_already_set& err)
    {
        LOG_ERROR("Py module error %s", err.what());
    }
}
void py_script::instantiate(entity ent)
{
    py::object instance = py_class(ent);
    lemon_assert(py::isinstance<scriptable_entity>(instance));
    s_entity = instance.cast<scriptable_entity*>();
}
} // namespace lemon
