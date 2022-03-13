#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

namespace lemon {
struct LEMON_PUBLIC box_collider
{
    vec2 center{ 0.f, 0.f };
    vec2 size{ 1.f, 1.f };
};
struct LEMON_PUBLIC circle_collider
{
    vec2 center{ 0.f, 0.f };
    f32 radius{ 1.f };
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
} // namespace lemon
