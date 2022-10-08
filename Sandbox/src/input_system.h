#pragma once

#include "events/events.h"
#include "world/service_container.h"
#include <world/system.h>

using namespace lemon;
class input_system : public system
{
  public:
    SYSTEM(input_system, registry, event_queue, input);
    ~input_system();
    void update() override;

  private:
    registry& _registry;
    event_queue& _eventQueue;
    input& _input;
    event_queue::listener_handle hor, vert;
};
