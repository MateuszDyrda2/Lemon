#pragma once

#include <lemon/scene/basic_components.h>
#include <lemon/scene/system.h>

#include <lemon/assets/asset.h>
#include <lemon/audio/audio_player.h>

namespace lemon {
class scene;
class LEMON_PUBLIC audio_system : public system
{
  public:
    audio_system(ptr<scene> s);
    ~audio_system();
    void update(entity_registry& registry) override;
    static void begin_play(entity ent);
    static void begin_play(entity ent, const asset<sound>& buffer);

  private:
    audio_player player;
    entity listener;
};
} // namespace lemon
