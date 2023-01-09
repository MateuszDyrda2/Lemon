/** @file animation_system.h
 * @brief File with a system responsible for animations
 */
#pragma once

#include <core/defines.h>
#include <events/events.h>
#include <world/system.h>

namespace lemon {
/** Animation system class */
class LEMON_API animation_system : public system
{
  public:
    SYSTEM(animation_system, scene, event_queue);
    ~animation_system();

    /** @brief Runs on each render
     * @param e event arguments
     */
    void on_render(event_args* e) noexcept;

  private:
    scene& _scene;
    event_queue::listener_handle update;
};
}
