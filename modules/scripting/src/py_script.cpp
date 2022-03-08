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
    LEMON_ASSERT(py::isinstance<scriptable_entity>(instance));
    try
    {
        auto c  = instance.attr("create");
        auto u  = instance.attr("update");
        auto lu = instance.attr("late_update");
        auto d  = instance.attr("destroy");
        auto e  = instance.attr("on_enable");
        auto de = instance.attr("on_disable");

        create     = c.cast<py::function>();
        update     = u.cast<py::function>();
        lateUpdate = lu.cast<py::function>();
        destroy    = d.cast<py::function>();
    }
    catch(const py::error_already_set& err)
    {
        LOG_ERROR("%s", err.what());
    }
    s_entity = instance.cast<scriptable_entity*>();
}
} // namespace lemon
