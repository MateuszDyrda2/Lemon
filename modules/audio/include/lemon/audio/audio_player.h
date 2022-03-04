#pragma once

#include "sound.h"
#include "sound_source.h"

#include <lemon/core/math/vec3.h>

namespace lemon {
/** OpenAL implementation for audio engine */
class LEMON_PUBLIC audio_player
{
  public:
    /** @brief Creates the audio engine */
    audio_player();
    ~audio_player();
    /** @brief Changes the state of the scene listener to prepare
     * for sound in this frame
     */
    void start_playing(f32 masterGain, const vec3& pos);
    /** @brief Plays the sound
     */
    void play(f32 pitch, f32 gain, bool loop, sound& s, sound_source& ss, const vec3& pos);
};
} // namespace lemon
