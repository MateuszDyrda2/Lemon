#include <lemon/audio/sound_source.h>

namespace lemon {
sound_source::sound_source()
{
    alCall(alGenSources, 1, &handle);
}
sound_source::~sound_source()
{
    if(handle)
    {
        alCall(alSourceStopv, 1, &handle);
        alCall(alDeleteSources, 1, &handle);
    }
}
sound_source::sound_source(const sound_source&)
{
    alCall(alGenSources, 1, &handle);
}
sound_source& sound_source::operator=(const sound_source& other)
{
    if(this != &other)
    {
        if(handle)
        {

            alCall(alSourceStopv, 1, &handle);
            alCall(alDeleteSources, 1, &handle);
        }
        alCall(alGenSources, 1, &handle);
    }
    return *this;
}
sound_source::sound_source(sound_source&& other) noexcept:
    handle(other.handle)
{
    other.handle = 0;
}
sound_source& sound_source::operator=(sound_source&& other) noexcept
{
    if(this != &other)
    {
        if(handle)
        {

            alCall(alSourceStopv, 1, &handle);
            alCall(alDeleteSources, 1, &handle);
        }

        handle       = other.handle;
        other.handle = 0;
    }
    return *this;
}
} // namespace lemon
