#pragma once

#include <lemon/game/basic_components.h>

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
     * @param lComponent listener component
     * @param tComponent transform component of the listener
     */
    void start_playing(audio_listener& lComponent, transform& tComponent);
    /** @brief Plays the sound
     * @param sComponent audio source component
     * @param tComponent transform component of the source
     */
    void play(audio_source& sComponent, transform& tComponent);
};
} // namespace lemon
