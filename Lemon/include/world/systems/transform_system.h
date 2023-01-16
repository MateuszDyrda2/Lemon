/** @file transform_system.h
 * @brief File with a transform system definition
 */
#pragma once

#include "../components/transform_components.h"
#include "../entity.h"
#include "../system.h"
#include "events/events.h"

#include <core/math/vec2.h>

namespace lemon {
/** System responsible for managing transformation */
class LEMON_API transform_system : public system
{
  public:
    SYSTEM(transform_system, scene, event_queue);
    ~transform_system();
    /** @brief Function called on each update
     * @param e event args
     */
    void on_update(event_args* e);
    /** @brief Get entity transform
     * @param _entity entity
     * @return entity transform reference
     */
    static transform& get_transform(entity _entity);
    /** @brief Move the entity to a new position
     * @param _entity entity
     * @param newPosition position to move the entity to
     */
    static void move_to(entity _entity, vec2 newPosition);
    /** @brief Move the entity by a vector
     * @param _entity entity
     * @param distance distance to move the entity
     */
    static void move_by(entity& _entity, vec2 distance);
    /** @brief Change entity rotation
     * @param _entity entity
     * @param newRotation new entity rotation
     */
    static void rotate_to(entity& _entity, f32 newRotation);
    /** @brief Rotate the entity
     * @param _entity entity
     * @param degrees rotation to apply
     */
    static void rotate_by(entity& _entity, f32 degrees);
    /** @brief Scale the entity
     * @param _entity entity
     * @param newScale new entity scale
     */
    static void scale_to(entity& _entity, vec2 newScale);
    /** @brief Scele the entity by
     * @param _entity entity
     * @param scale value to scale by
     */
    static void scale_by(entity& _entity, vec2 scale);
    /** @brief Linear interpolation
     * @param start starting position
     * @param end end position
     * @param time time to lerp
     * @return resulting vector
     */
    static vec2 lerp(vec2 start, vec2 end, f32 time);

  private:
    scene& _scene;
    event_queue::listener_handle update;
};
}
