#pragma once

#include <world/system.h>

using namespace lemon;
class movement_system : public system
{
  public:
    SYSTEM(movement_system, scene, event_queue);
    ~movement_system();
    void onUpdate(event_args* e);

  private:
    event_queue::listener_handle horizontal, vertical;
};
