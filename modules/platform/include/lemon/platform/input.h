#pragma once

#include "input_device.h"
#include "key_codes.h"
#include "window.h"

#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

#include <vector>

namespace lemon {
class LEMON_PUBLIC input
{
  public:
    input(window& window);
    void update();
    ~input();
    bool is_key_pressed(key::keycode key) noexcept;
    bool is_gamepad_key_pressed(key::gamepad key) noexcept;
    bool is_mouse_pressed(key::mouse button) noexcept;
    f32 get_gamepad_axis(key::axis axis) noexcept;
    vec2 get_mouse_pos() noexcept;
    f32 get_horizontal() noexcept;
    f32 get_vertical() noexcept;

  private:
    window& win;
};
} // namespace lemon
