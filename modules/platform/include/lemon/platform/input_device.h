#pragma once

#include "key_codes.h"

#include <lemon/core/basic_types.h>

#include <unordered_map>

namespace lemon {
using key_state = f32;
struct input_device
{
    enum class type
    {
        KEYBOARD,
        MOUSE,
        GAMEPAD
    };

    size_type index;
    type deviceType;
};
} // namespace lemon
