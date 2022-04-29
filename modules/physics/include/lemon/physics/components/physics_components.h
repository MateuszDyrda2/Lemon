#pragma once

#include "../SAT.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>
#include <lemon/scene/reflection.h>

#include <utility>
#include <vector>

namespace lemon {
struct LEMON_PUBLIC rigidbody : public component
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
    f32 linearDrag{};
    f32 angularDrag{};
    f32 mass{ 1.0f };
    f32 gravityScale{ 10.f };
    bool freezeRotation{};
    body_type bodyType{};
    collision_detection collisionDetection{};

    rigidbody()  = default;
    ~rigidbody() = default;

    LEMON_REFLECT(rigidbody, velocity, angularVelocity, linearDrag, angularDrag,
                  mass, gravityScale, freezeRotation, bodyType, collisionDetection);
};
struct LEMON_PUBLIC collider : public component
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
    f32 bounciness{};
    f32 friction{};
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
    ~collider() = default;
    LEMON_REFLECT(collider, shape, bounciness, friction);
};
struct LEMON_PUBLIC collided
{
    std::vector<std::pair<entity_handle, MTV>> with;
};
struct LEMON_PUBLIC collision_m
{
    entity_handle A;
    entity_handle B;
    MTV mtv;
};
struct LEMON_PUBLIC trigger_m
{
    entity_handle A;
    entity_handle B;
};
struct LEMON_PUBLIC is_trigger_t
{ };
struct LEMON_PUBLIC collision_enter_m
{
    entity_handle A{ entt::null };
    entity_handle B{ entt::null };
};
struct LEMON_PUBLIC collision_leave_m
{
    entity_handle A{ entt::null };
    entity_handle B{ entt::null };
};
} // namespace lemon
