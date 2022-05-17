#pragma once

#include <lemon/core/time/game_clock.h>
#include <lemon/scene/system.h>
#include <lemon/threading/scheduler.h>

#include <unordered_set>

namespace lemon {
class scene;
class collision_response_system : public system
{
  public:
    collision_response_system(service_registry& globalRegistry);
    ~collision_response_system();

    void on_update(entity_registry& registry) override;

  private:
    game_clock& clk;
    scheduler& sch;
    std::unordered_set<u64> cachedCollisions;
    std::unordered_set<u64> cachedTriggers;
};
} // namespace lemon
