#pragma once

#include "../system.h"

#include <lemon/core/defines.h>
#include <lemon/core/time/clock.h>

namespace lemon {
class LEMON_PUBLIC time_system : public system
{
  public:
    time_system();
    ~time_system();
    void update(entity_registry registry) override;

  private:
    clock gameClock;
};
} // namespace lemon
