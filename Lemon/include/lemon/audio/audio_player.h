#pragma once

#include <lemon/game/basic_components.h>

namespace lemon {
class audio_player
{
  public:
    audio_player();
    ~audio_player();
    void start_playing(audio_listener& lComponent, transform& tComponent);
    void play(audio_source& sComponent, transform& tComponent);
};
} // namespace lemon
