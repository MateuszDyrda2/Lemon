#pragma once

#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/vec2.h>

namespace lemon {
enum collider_type
{
    none,
    box,
    circle,
    capsule,
};
struct LEMON_API [[lemon::component]] rigidbody
{
    [[lemon::field]] vec2 position              = {};
    [[lemon::field]] f32 rotation               = {};
    [[lemon::field]] vec2 velocity              = {};
    [[lemon::field]] f32 angularRotation        = {};
    [[lemon::field]] f32 linearDrag             = {};
    [[lemon::field]] f32 mass                   = { 1.f };
    [[lemon::field]] bool isKinetic             = { true };
    [[lemon::field]] collider_type colliderType = collider_type::none;

    vec2 _oldPosition = {};
};

/** @note Don't change collider properties after assignment! */
struct LEMON_API [[lemon::component]] box_collider
{
    [[lemon::field]] f32 bounciness = {};
    [[lemon::field]] f32 friction   = {};
    [[lemon::field]] vec2 offset    = {};
    [[lemon::field]] vec2 hSize     = { 0.5f, 0.5f };

    box_collider(vec2 offset, vec2 hSize):
        offset(offset), hSize(hSize) { }
    box_collider() = default;
    box_collider(f32 bounciness, f32 friction, vec2 offset, vec2 hSize):
        bounciness(bounciness), friction(friction), offset(offset), hSize(hSize) { }
};

/** @note Don't change collider properties after assignment! */
struct LEMON_API [[lemon::component]] circle_collider
{
    [[lemon::field]] f32 bounciness = {};
    [[lemon::field]] f32 friction   = {};
    [[lemon::field]] vec2 offset    = {};
    [[lemon::field]] f32 radius     = { 0.5f };
};

/** @note Don't change collider properties after assignment! */
struct LEMON_API [[lemon::component]] capsule_collider
{
    [[lemon::field]] f32 bounciness = {};
    [[lemon::field]] f32 friction   = {};
    [[lemon::field]] vec2 offset    = {};
    [[lemon::field]] f32 radius     = { 0.5f };
    [[lemon::field]] f32 height     = { 0.5f };
};
}
