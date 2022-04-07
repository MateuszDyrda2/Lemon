#include <lemon/engine/py_bindings/py_events.h>

#include <lemon/events/event_bus.h>

#include <pybind11/embed.h>

namespace lemon {
void py_init_events()
{
}
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(events, m)
{
    /*py::class_<event_args>(m, "event_args")
        .def(py::init<>());
    py::class_<event>(m, "event")
        .def(py::init<>())
        .def("notify", &event::notify)
        .def("register", &event::register_observer)
        .def("unregister", &event::unregister_observer);*/
}
} // namespace lemon
