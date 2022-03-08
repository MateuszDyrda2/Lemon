#include <lemon/engine/py_bindings/py_audio.h>

#include <lemon/audio/audio_player.h>
#include <lemon/audio/sound.h>
#include <lemon/audio/sound_context.h>
#include <lemon/audio/sound_source.h>

#include <pybind11/embed.h>

namespace lemon {
void py_init_audio()
{
}
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(audio, m)
{
    py::class_<audio_player>(m, "audio_player")
        .def("play", &audio_player::start_playing);
    py::class_<sound>(m, "sound")
        .def_property_readonly("sampleRate", [](const sound& a) { return a.get_info().sampleRate; })
        .def_property_readonly("bitDepth", [](const sound& a) { return a.get_info().bitDepth; })
        .def_property_readonly("lenght", [](const sound& a) { return a.get_info().lenght; })
        .def_property_readonly("channelCount", [](const sound& a) { return a.get_info().channelCount; });
}
} // namespace lemon
