#pragma once

#include <core/defines.h>
#include <core/lemon_types.h>
#include <world/reflection.h>

namespace lemon {
struct LEMON_API [[lemon::component]] rigidbody
{
    [[lemon::field]] vec2 position       = {};
    [[lemon::field]] f32 rotation        = {};
    [[lemon::field]] vec2 velocity       = {};
    [[lemon::field]] f32 angularRotation = {};
    [[lemon::field]] f32 linearDrag      = {};
    [[lemon::field]] f32 mass            = { 1.f };
    [[lemon::field]] bool isKinetic      = { true };
};

struct LEMON_API [[lemon::component]] box_collider
{
    [[lemon::field]] f32 bounciness = {};
    [[lemon::field]] f32 friction   = {};
    [[lemon::field]] vec2 offset    = {};
    [[lemon::field]] vec2 hSize     = { 0.5f, 0.5f };
};

struct LEMON_API [[lemon::component]] circle_collider
{
    [[lemon::field]] f32 bounciness = {};
    [[lemon::field]] f32 friction   = {};
    [[lemon::field]] vec2 offset    = {};
    [[lemon::field]] f32 radius     = { 0.5f };
};

struct LEMON_API [[lemon::component]] capsule_collider
{
    [[lemon::field]] f32 bounciness = {};
    [[lemon::field]] f32 friction   = {};
    [[lemon::field]] vec2 offset    = {};
    [[lemon::field]] f32 radius     = { 0.5f };
    [[lemon::field]] f32 height     = { 0.5f };
};

LEMON_REFL(rigidbody, position, rotation, velocity, angularRotation, linearDrag, mass, isKinetic);
LEMON_REFL(box_collider, bounciness, friction, offset, hSize);
LEMON_REFL(circle_collider, bounciness, friction, offset, radius);
LEMON_REFL(capsule_collider, bounciness, friction, offset, radius, height);
}
