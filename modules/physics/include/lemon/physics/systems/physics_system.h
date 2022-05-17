#pragma once

#include <lemon/core/time/game_clock.h>
#include <lemon/scene/system.h>
#include <lemon/threading/scheduler.h>

namespace lemon {
class scene;
class physics_system : public system
{
  public:
    physics_system(service_registry& globalRegistry);
    ~physics_system();
    void on_update(entity_registry& registry) override;

  private:
    game_clock& clk;
    scheduler& sch;
};
}

