#include <lemon/engine/py_bindings/py_threading.h>

#include <lemon/threading/scheduler.h>
#include <pybind11/embed.h>

namespace lemon {
void py_init_threading()
{
}
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(threading, m)
{
    /*    py::class_<scheduler>(m, "scheduler")
            .def_static("run", &scheduler::run, py::call_guard<py::gil_scoped_release>())
            .def_static("run_after", &scheduler::run_after, py::call_guard<py::gil_scoped_release>())
            .def_static("run_at", &scheduler::run_at, py::call_guard<py::gil_scoped_release>());*/
}
} // namespace lemon
