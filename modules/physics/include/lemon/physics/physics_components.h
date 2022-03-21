#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

namespace lemon {
struct LEMON_PUBLIC physics_changed
{ };

struct LEMON_PUBLIC rigidbody
{
    enum class collision_detection
    {
        Discrete,
        Continuous
    };
    enum class body_type
    {
        Dynamic,
        Kinematic
    };

    vec2 velocity{};
    f32 angularVelocity{};
    vec2 force{};
    f32 torque{};
    f32 linearDrag{};
    f32 angularDrag{};
    f32 mass{ 1.0f };
    f32 gravityScale{ 10.f };
    bool freezeRotation{};
    body_type bodyType{};
    collision_detection collisionDetection{};
};
struct LEMON_PUBLIC collider
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
    collider() = default;
    collider(vec2 offset, f32 radius):
        shape(Circle), offset(offset), circle() { circle.radius = radius; }
    collider(vec2 offset, vec2 hSize):
        shape(Box), offset(offset), box() { box.hSize = hSize; }
    collider(vec2 offset, f32 radius, f32 hHeight):
        shape(Capsule), offset(offset), capsule()
    {
        capsule.radius  = radius;
        capsule.hHeight = hHeight;
    }
};
} // namespace lemon
