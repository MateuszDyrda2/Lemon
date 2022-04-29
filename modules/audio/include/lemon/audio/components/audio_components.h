#pragma once

#include "../sound.h"
#include "../sound_source.h"

#include <lemon/assets/asset.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/scene/reflection.h>

namespace lemon {
/* Play tag attached to the entity with audio_source
 * that wants to play a sound in a specific frame */
struct LEMON_PUBLIC play
{ };
/* Sound source in the game world */
struct LEMON_PUBLIC audio_source : public component
{
    sound_source source;
    asset<sound> clip{};
    f32 pitch{ 1.0f };
    f32 gain{ 1.0f };
    bool loop{ false };
    audio_source()  = default;
    ~audio_source() = default;

    LEMON_REFLECT(audio_source, source, clip, pitch, gain, loop);
};
/* Sound listener in the game world (One must be available in the scene) */
struct LEMON_PUBLIC audio_listener : public component
{
    f32 masterGain{ 1.0f };
    audio_listener() = default;
    audio_listener(f32 master):
        masterGain(master) { }
    ~audio_listener() = default;

    LEMON_REFLECT(audio_listener, masterGain);
};
} // namespace lemon
