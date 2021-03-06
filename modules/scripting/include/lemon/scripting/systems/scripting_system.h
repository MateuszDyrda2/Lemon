#pragma once

#include <lemon/scene/system.h>

#include <lemon/core/time/clock.h>
#include <lemon/scripting/py_engine.h>

namespace lemon {
class scene;
class LEMON_PUBLIC scripting_system : public system
{
  public:
    scripting_system(ptr<scene> s, clock& clk);
    ~scripting_system();
    void update(entity_registry& registry) override;

  private:
    clock& clk;
};
} // namespace lemon
