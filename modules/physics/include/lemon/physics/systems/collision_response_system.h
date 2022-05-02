#pragma once

#include <lemon/core/time/clock.h>
#include <lemon/scene/system.h>
#include <lemon/threading/scheduler.h>

#include <unordered_set>

namespace lemon {
class scene;
class collision_response_system : public system
{
  public:
    collision_response_system(ptr<scene> s, clock& clk, scheduler& sch);
    ~collision_response_system();

    void update(entity_registry& registry) override;

  private:
    clock& clk;
    scheduler& sch;
    std::unordered_set<u64> cachedCollisions;
    std::unordered_set<u64> cachedTriggers;
};
} // namespace lemon
