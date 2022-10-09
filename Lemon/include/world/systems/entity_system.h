#pragma once

#include "../components/entity_components.h"
#include "../system.h"

namespace lemon {
class LEMON_API entity_system : public system
{
  public:
    SYSTEM(entity_system, scene, event_queue);
    ~entity_system();
    void onEarlyUpdate(event_args* e);

  private:
    scene& _scene;
    event_queue::listener_handle update;
};
}
