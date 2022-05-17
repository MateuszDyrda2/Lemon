#pragma once

#include "../audio_player.h"

#include <lemon/assets/asset.h>
#include <lemon/scene/system.h>

namespace lemon {
class scene;
class LEMON_PUBLIC audio_system : public system
{
  public:
    audio_system(service_registry& globalRegistry);
    ~audio_system() = default;
    void on_update(entity_registry& registry) override;
    static void begin_play(entity ent);
    static void begin_play(entity ent, const asset<sound>& buffer);

  private:
    audio_player player;
    entity listener;
};
} // namespace lemon
