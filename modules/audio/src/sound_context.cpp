#include <lemon/audio/sound_context.h>

namespace lemon {
void sound_context::create()
{
    // TODO: enumerate available devices
    openALDevice = alcOpenDevice(nullptr);
    if(!openALDevice)
    {
        LOG_ERROR("No default sound device found!");
    }
    if(!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr)
       || !openALContext)
    {
        LOG_ERROR("Could not create auto context");
    }
    ALCboolean contextMadeCurrent = false;
    if(!alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext)
       || contextMadeCurrent != ALC_TRUE)
    {
        LOG_ERROR("Could not make audio context current");
    }
}
void sound_context::drop()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(openALContext);

    alcCloseDevice(openALDevice);
}

} // namespace lemon
