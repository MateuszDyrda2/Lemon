#pragma once

#include "events/events.h"
#include "world/service_container.h"
#include <world/system.h>

using namespace lemon;
class input_system : public system
{
  public:
    SYSTEM(input_system, scene, input);
    ~input_system();

  private:
    registry& _registry;
    input& _input;
};
