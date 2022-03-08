#pragma once

#include "key_codes.h"
#include "window_base.h"

#include <lemon/core/basic_types.h>

#include <lemon/core/math/vec2.h>

namespace lemon {
class LEMON_PUBLIC input
{
  public:
    input(ptr<window_base> window);
    ~input();
    [[nodiscard]] static bool is_key_pressed(key::keycode key);
    [[nodiscard]] static bool is_mouse_pressed(key::mouse button);
    [[nodiscard]] static vec2 get_mouse_pos();

  private:
    static ptr<window_base> _window;
};
} // namespace lemon
