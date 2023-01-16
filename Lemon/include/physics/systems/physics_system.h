/** @file physics_system.h
 * @brief File containing definition of a physics system
 */
#pragma once

#include <events/events.h>
#include <world/entity.h>
#include <world/system.h>

namespace lemon {
/** class responsible for collisions in the engine */
class LEMON_API physics_system : public system
{
  public:
    SYSTEM(physics_system, scene, event_queue);
    ~physics_system();
    /** @brief Funciton called on each update
     * @param e event arguments
     */
    void on_update(event_args* e);
    /** @brief Sets entity velocity
     * @param _entity entity to set velocity of
     * @param newVelocity velocity to change
     */
    static void set_velocity(entity _entity, vec2 newVelocity);
    /** @brief Adds velocity to entity
     * @param _entity entity to set velocity of
     * @param velocity velocity to add
     */
    static void add_velocity(entity _entity, vec2 velocity);
    /** @brief Calculates the velocity needed to move the entity to reach the position
     * @param _entity entity to set velocity of
     * @param newPosiiton new position to set
     * @param deltaTime fixed delta time
     */
    static void move_position(entity _entity, vec2 newPosition, f32 deltaTime);

  private:
    scene& _scene;
    event_queue::listener_handle update;
};
}
