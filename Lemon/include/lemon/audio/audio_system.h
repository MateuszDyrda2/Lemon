#pragma once

#include "audio_player.h"
#include <lemon/game/system.h>

namespace lemon {
class scene;
class LEMON_PUBLIC audio_system : public system
{
  public:
    audio_system(ptr<scene> s);
    ~audio_system();
    void update(entity_registry& registry) override;

  private:
    audio_player player;
	entity listener;
};
} // namespace lemon