#include <lemon/scripting/py_engine.h>

#include <lemon/core/game.h>
#include <pybind11/embed.h>

namespace lemon {
namespace py = pybind11;
py_engine::py_engine()
{
    py::initialize_interpreter();
    try
    {
        py::module_::import("sys")
            .attr("path")
            .attr("append")(
                game::get_settings().workingDirectory
                + "/"
                + game::get_settings().assetPath
                + "/scripts");
    }
    catch(const py::error_already_set& err)
    {
        LOG_ERROR("Python failed with: %s", err.what());
    }
}
py_engine::~py_engine()
{
    py::finalize_interpreter();
}

} // namespace lemon
