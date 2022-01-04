#pragma once

#include <lemon/game/system.h>
#include <lemon/time/clock.h>

namespace lemon {
class scene;
class scripting_system : public system
{
  public:
    scripting_system(ptr<scene> s);
    ~scripting_system();
    void update(entity_registry& registry) override;

  private:
    ptr<clock> clk;
};
} // namespace lemon
