#include <lemon/scripting/py_script.h>

namespace lemon {
py_script::py_script(const std::string& moduleName)
{
    mod = py::module_::import(moduleName.c_str());
    try
    {
        onCreate     = mod.attr("create");
        onUpdate     = mod.attr("update");
        onLateUpdate = mod.attr("late_update");
        onDestroy    = mod.attr("destroy");
    }
    catch(const py::error_already_set& err)
    {
        LOG_ERROR("Py module error %s", err.what());
    }
}
void py_script::instantiate(entity ent)
{
    this->ent       = ent;
    mod.attr("ent") = py::cast(ent);
}
} // namespace lemon
