#pragma once

#include "system.h"

#include <lemon/core/time/clock.h>
#include <lemon/scripting/py_engine.h>

namespace lemon {
class scene;
class LEMON_PUBLIC scripting_system : public system
{
  public:
    scripting_system(ptr<scene> s);
    ~scripting_system();
    void update(entity_registry& registry) override;

  private:
    ptr<clock> clk;
    py_engine python_engine;
};
} // namespace lemon
