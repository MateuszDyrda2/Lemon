#pragma once

#include <lemon/core/time/game_clock.h>
#include <lemon/scene/system.h>
#include <lemon/threading/scheduler.h>

namespace lemon {
class scene;
class LEMON_PUBLIC movement_system : public system
{
  public:
    movement_system(service_registry& globalRegistry);
    ~movement_system() = default;

    void on_update(entity_registry& registry) override;

  private:
    scheduler& sch;
    game_clock& clk;
};
} // namespace lemon
