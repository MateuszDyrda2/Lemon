#include <lemon/audio/sound_source.h>

namespace lemon {

sound_source::sound_source()
{
    alCall(alGenSources, 1, &handle);
}
sound_source::~sound_source()
{
    alCall(alDeleteSources, 1, &handle);
}
} // namespace lemon
