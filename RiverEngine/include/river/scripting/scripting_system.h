#pragma once

#include <river/game/system.h>
#include <river/time/clock.h>

namespace river {
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
} // namespace river
