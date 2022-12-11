#pragma once

#include "../components/entity_components.h"
#include "../system.h"
#include "scripting/message_bus.h"

namespace lemon {
class LEMON_API entity_system : public system
{
  public:
    SYSTEM(entity_system, scene, event_queue, message_bus);
    ~entity_system();
    void onEarlyUpdate(event_args* e);
    void on_mount(event_args* e);

  private:
    scene& _scene;
    message_bus& _messageBus;
    event_queue::listener_handle update;
    event_queue::listener_handle mount;

  private:
    void add_to_registry(registry& _reg, entity_t handle);
};
}
