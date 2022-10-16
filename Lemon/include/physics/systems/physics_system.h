#pragma once

#include <events/events.h>
#include <world/entity.h>
#include <world/system.h>

namespace lemon {
class LEMON_API physics_system : public system
{
  public:
    SYSTEM(physics_system, scene, event_queue);
    ~physics_system();
    void onUpdate(event_args* e);

  private:
    scene& _scene;
    event_queue::listener_handle update;
};
}
