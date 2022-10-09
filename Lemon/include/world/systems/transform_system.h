#pragma once

#include "../components/transform_components.h"
#include "../entity.h"
#include "../system.h"
#include "events/events.h"

#include <core/math/vec2.h>

namespace lemon {
class LEMON_API transform_system : public system
{
  public:
    SYSTEM(transform_system, scene, event_queue);
    ~transform_system();
    void onUpdate(event_args* e);

    static void move_to(entity& _entity, vec2 newPosition);
    static void move_by(entity& _entity, vec2 distance);
    static void rotate_to(entity& _entity, f32 newRotation);
    static void rotate_by(entity& _entity, f32 degrees);
    static void scale_to(entity& _entity, vec2 newScale);
    static void scale_by(entity& _entity, vec2 scale);

  private:
    registry& _registry;
    event_queue::listener_handle update;
};
}
