#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/time/clock.h>
#include <lemon/scene/system.h>
#include <lemon/threading/scheduler.h>

namespace lemon {
class scene;
class LEMON_PUBLIC gravity_system : public system
{
  public:
    gravity_system(ptr<scene> s, scheduler& sch, clock& clk);
    ~gravity_system();
    void update(entity_registry& registry) override;

  private:
    f32 gravity;
    scheduler& sch;
    clock& clk;
};

} // namespace lemon
