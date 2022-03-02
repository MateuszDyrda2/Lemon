#include <lemon/audio/audio_player.h>

namespace lemon {
audio_player::audio_player()
{
}
audio_player::~audio_player()
{
}
void audio_player::start_playing(audio_listener& lComponent, transform& tComponent)
{
    alCall(alListenerf, AL_GAIN, lComponent.masterGain);
    alCall(alListener3f, AL_POSITION, tComponent.position.x, tComponent.position.y, tComponent.position.z);
}
void audio_player::play(audio_source& sComponent, transform& tComponent)
{
    auto sound  = sComponent.sound.get();
    auto source = &sComponent.source;

    alCall(alSourcef, source->get_handle(), AL_PITCH, sComponent.pitch);
    alCall(alSourcef, source->get_handle(), AL_GAIN, sComponent.gain);
    alCall(alSource3f, source->get_handle(), AL_POSITION, tComponent.position.x, tComponent.position.y, tComponent.position.y);
    alCall(alSourcei, source->get_handle(), AL_LOOPING, sComponent.loop);
    alCall(alSourcei, source->get_handle(), AL_BUFFER, sound->get_handle());

    alCall(alSourcePlay, source->get_handle());
}

} // namespace lemon