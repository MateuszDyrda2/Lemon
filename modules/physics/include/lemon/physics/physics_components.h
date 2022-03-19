#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

namespace lemon {
struct LEMON_PUBLIC rigidbody
{
    vec2 velocity{};
    f32 angularVelocity{};
    vec2 force{};
    f32 torque{};
    f32 mass{ 1.0f };
    f32 gravityScale{ 10.f };
};
struct collider
{
    enum collision_shape : byte
    {
        Box,
        Circle,
        Capsule
    } shape{ Box };
    vec2 offset{ 0.0f, 0.0f };
    union
    {
        struct
        {
            f32 radius{ 0.5f };
        } circle;
        struct
        {
            vec2 hSize{ 0.5f, 0.5f };
        } box;
        struct
        {
            f32 radius{ 0.5f }, hHeight{ 0.5f };
        } capsule;
    };
};
} // namespace lemon
