#include <lemon/scripting/py_engine.h>

#include <lemon/core/game.h>

namespace lemon {
py_engine::py_engine():
    _interpreter{}
{
    try
    {
        py::module_::import("sys")
            .attr("path")
            .attr("append")(
                game::get_settings().workingDirectory
                + "/"
                + game::get_settings().assetPath
                + "/scripts");
        auto test = py::module_::import("example");
        auto func = test.attr("test_lemon");
        func();
    }
    catch(const py::error_already_set& err)
    {
        LOG_ERROR("Python failed with: %s", err.what());
    }
}
py_engine::~py_engine()
{
}

} // namespace lemon
