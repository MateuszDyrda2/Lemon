#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/string_id.h>
#include <lemon/platform/key_codes.h>
#include <lemon/scene/reflection.h>

#include <utility>
#include <vector>

namespace lemon {
struct LEMON_PUBLIC player_input
{
    std::vector<std::pair<key::keycode, f32>> moveForward;
    std::vector<std::pair<key::keycode, f32>> moveSideways;
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
