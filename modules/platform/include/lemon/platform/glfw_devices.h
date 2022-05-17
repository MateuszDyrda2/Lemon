#pragma once

#include "input_device.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>

#include <unordered_map>

namespace lemon {
struct LEMON_PUBLIC glfw_devices
{
    std::unordered_map<key::keycode, key_state> keyboardState;
    std::unordered_map<key::keycode, key_state> mouseState;
    std::unordered_map<size_type, std::unordered_map<key::keycode, key_state>> gamepadState;
};
} // namespace lemon
