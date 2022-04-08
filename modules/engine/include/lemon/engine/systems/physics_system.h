#pragma once

#include <lemon/core/math/vec4.h>
#include <lemon/core/time/clock.h>
#include <lemon/scene/system.h>
#include <lemon/threading/scheduler.h>

namespace lemon {
class scene;
class LEMON_PUBLIC physics_system : public system
{
  public:
    physics_system(ptr<scene> s, clock& clk, scheduler& sch);
    ~physics_system();

    void update(entity_registry& registry) override;

  private:
    clock& clk;
    scheduler& sch;
};
} // namespace lemon