/** @file interpolate_system.h
 * @brief File with interpolation system definition
 */
#pragma once

#include "../system.h"
#include "world/scene.h"

namespace lemon {
/** System interpolating physics each frame */
class LEMON_API interpolate_system : public system
{
  public:
    SYSTEM(interpolate_system, scene, event_queue);
    ~interpolate_system();
    /** @brief Function called each frome on update
     * @param e event args
     */
    void on_update(event_args* e);

  private:
    scene& _scene;
    event_queue::listener_handle update;
};
}
