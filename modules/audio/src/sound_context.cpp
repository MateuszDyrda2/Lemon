#include <lemon/audio/sound_context.h>

namespace lemon {
ptr<ALCdevice> sound_context::openALDevice   = nullptr;
ptr<ALCcontext> sound_context::openALContext = nullptr;
sound_context::sound_context()
{
    create();
}
sound_context::~sound_context()
{
    drop();
}
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
    LOG_MESSAGE("Sound context created");
}
void sound_context::drop()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(openALContext);

    alcCloseDevice(openALDevice);
    LOG_MESSAGE("Sound context destroyed");
}

} // namespace lemon
