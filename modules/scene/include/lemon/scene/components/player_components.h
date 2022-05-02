#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/string_id.h>
#include <lemon/platform/key_codes.h>
#include <lemon/scene/reflection.h>

#include <unordered_map>

namespace lemon {
struct LEMON_PUBLIC player_input
{
    key::keycode moveUp;
    key::keycode moveDown;
    key::keycode moveLeft;
    key::keycode moveRight;

    player_input() = default;
    player_input(key::keycode moveUp,
                 key::keycode moveDown,
                 key::keycode moveLeft,
                 key::keycode moveRight):
        moveUp(moveUp),
        moveDown(moveDown),
        moveLeft(moveLeft),
        moveRight(moveRight) { }
};
struct LEMON_PUBLIC entity_controller
{
    float speed{ 1.f };
};
struct LEMON_PUBLIC move_m
{
    vec2 direction;
};
} // namespace lemon
