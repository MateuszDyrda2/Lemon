#include <lemon/engine/py_bindings/py_events.h>

#include <lemon/events/event_handler.h>

#include <pybind11/embed.h>

namespace lemon {
void py_init_events()
{
}
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(events, m)
{
    py::class_<event_base>(m, "event")
        .def(py::init<>());
    py::class_<event_handler>(m, "handler")
        .def_static("fire",
                    py::overload_cast<
                        const std::string&, event_base*>(
                        &event_handler::fire_event))
        .def_static("subscribe",
                    py::overload_cast<
                        const std::string&, const std::function<void(event_base*)>&>(
                        &event_handler::subscribe));
    m.def("on_event", [](const std::string& name) {
        std::function<std::function<void(event_base*)>(const std::function<void(event_base*)>&)> f =
            [name](const std::function<void(event_base*)>& fun) {
                event_handler::subscribe(name, fun);
                return fun;
            };
        return f;
    });
}
} // namespace lemon
