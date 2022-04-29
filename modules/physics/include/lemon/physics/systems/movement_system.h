#pragma once

#include <lemon/scene/system.h>
#include <lemon/threading/scheduler.h>
#include <lemon/core/time/clock.h>

namespace lemon {
class scene;
class LEMON_PUBLIC movement_system : public system
{
  public:
    movement_system(ptr<scene> s, clock& clk,  scheduler& sch);
    ~movement_system();

    void update(entity_registry& registry) override;

  private:
    scheduler& sch;
	clock& clk;
};
} // namespace lemon
