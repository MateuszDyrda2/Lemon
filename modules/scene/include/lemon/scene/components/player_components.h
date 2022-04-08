#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/string_id.h>
#include <lemon/platform/key_codes.h>
#include <lemon/scene/reflection.h>

#include <unordered_map>

namespace lemon {
struct LEMON_PUBLIC player_controller : public component
{
    std::unordered_map<key::keycode, string_id> mapping;
    player_controller() = default;
    player_controller(const std::unordered_map<key::keycode, string_id>& mapping):
        mapping(mapping) { }
    ~player_controller() = default;

    LEMON_REFLECT(player_controller, mapping);
};
} // namespace lemon
