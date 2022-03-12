#include <lemon/audio/audio_player.h>

namespace lemon {
audio_player::audio_player()
{
}
audio_player::~audio_player()
{
}
void audio_player::start_playing(f32 masterGain, const vec2& pos)
{
    alCall(alListenerf, AL_GAIN, masterGain);
    alCall(alListener3f, AL_POSITION, pos.x, pos.y, 0.0f);
}
void audio_player::play(f32 pitch, f32 gain, bool loop, sound& s, sound_source& ss, const vec2& pos)
{
    alCall(alSourcef, ss.get_handle(), AL_PITCH, pitch);
    alCall(alSourcef, ss.get_handle(), AL_GAIN, gain);
    alCall(alSource3f, ss.get_handle(), AL_POSITION, pos.x, pos.y, 0.0f);
    alCall(alSourcei, ss.get_handle(), AL_LOOPING, loop);
    alCall(alSourcei, ss.get_handle(), AL_BUFFER, s.get_handle());

    alCall(alSourcePlay, ss.get_handle());
}

} // namespace lemon