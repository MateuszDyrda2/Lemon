/** @file entity_system.h
 * @brief File with entity system definition
 */
#pragma once

#include "../components/entity_components.h"
#include "../system.h"
#include "scripting/message_bus.h"

namespace lemon {
/** System managing entity states */
class LEMON_API entity_system : public system
{
  public:
    SYSTEM(entity_system, scene, event_queue, message_bus);
    ~entity_system();
    /** @brief Function called on each early update
     * @param e event arguments
     */
    void on_early_update(event_args* e);
    /** @brief Function called on mount
     * @param e event args
     */
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
