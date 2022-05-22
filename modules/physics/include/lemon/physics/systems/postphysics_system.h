#pragma once

#include <lemon/core/time/game_clock.h>
#include <lemon/scene/system.h>

namespace lemon {
class postphysics_system : public system
{
    postphysics_system(service_registry& globalRegistry);
    ~postphysics_system();
    void on_update(entity_registry& registry) override;

  private:
    game_clock& clk;
};
} // namespace lemon
