#pragma once

#include <world/system.h>

using namespace lemon;
class movement_system : public system
{
  public:
    SYSTEM(movement_system, scene, event_queue, input);
    ~movement_system();
    void onUpdate(event_args* e);

  private:
    input& _input;
    scene& _scene;
    event_queue::listener_handle horizontal, vertical;
    event_queue::listener_handle update;
};
