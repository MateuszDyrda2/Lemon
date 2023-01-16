/** @file scripting_system.h
 * @brief File with a definition of a scripting system
 */
#pragma once

#include "../message_bus.h"

#include <core/defines.h>
#include <events/events.h>
#include <world/service_container.h>
#include <world/system.h>

namespace lemon {
/** Scripting system class */
class LEMON_API scripting_system : public system
{
  public:
    SYSTEM(scripting_system, scene, event_queue, message_bus);
    ~scripting_system();

    /** @brief Function run on update
     * @param e event args
     */
    void on_update(event_args* e);
    /** @brief Function run on early update
     * @param e event args
     */
    void on_early_update(event_args* e);
    /** @brief Function run on physics update
     * @param e event args
     */
    void on_physics(event_args* e);
    /** @brief Function run on mount
     * @param e event args
     */
    void on_mount(event_args* e);

  private:
    scene& _scene;
    message_bus& _messageBus;
    event_queue::listener_handle update;
    event_queue::listener_handle early;
    event_queue::listener_handle physics;
    event_queue::listener_handle mount;

  private:
    void add_start(registry& _registry, entity_t handle);
};
}
