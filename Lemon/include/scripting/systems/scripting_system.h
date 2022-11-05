#pragma once

#include "../message_bus.h"

#include <core/defines.h>
#include <events/events.h>
#include <world/service_container.h>
#include <world/system.h>

namespace lemon {
class LEMON_API scripting_system : public system
{
  public:
    SYSTEM(scripting_system, scene, event_queue, message_bus);
    ~scripting_system();

    void on_update(event_args* e);

  private:
    scene& _scene;
    message_bus& _messageBus;
    event_queue::listener_handle update;
};
}
