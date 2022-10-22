#pragma once

#include "../system.h"
#include "world/scene.h"

namespace lemon {
class LEMON_API interpolate_system : public system
{
  public:
    SYSTEM(interpolate_system, scene, event_queue);
    ~interpolate_system();

    void onUpdate(event_args* e);

  private:
    scene& _scene;
    event_queue::listener_handle update;
};
}
