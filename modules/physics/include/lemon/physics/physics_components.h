#pragma once

#include "colliders.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

namespace lemon {

struct LEMON_PUBLIC collider_component
{
    owned<collider_base> collider;
    struct AABB
    {
        vec2 min, max;
    } aabb;
};
struct LEMON_PUBLIC rigidbody
{
    vec2 velocity{};
    f32 angularVelocity{};
    vec2 force{};
    f32 torque{};
    f32 mass{ 1.0f };
    f32 gravityScale{ 10.f };
};
static bool detect_collision(ptr<box_collider> lhs, ptr<box_collider> rhs);
static bool detect_collision(ptr<box_collider> lhs, ptr<circle_collider> rhs);
static bool detect_collision(ptr<circle_collider> lhs, ptr<box_collider> rhs);
static bool detect_collision(ptr<circle_collider> lhs, ptr<circle_collider> rhs);

} // namespace lemon
