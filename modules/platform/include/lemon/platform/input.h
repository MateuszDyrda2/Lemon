#pragma once

#include "key_codes.h"
#include "window.h"

#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

namespace lemon {
class LEMON_PUBLIC input
{
  public:
    input(ptr<window> window);
    [[nodiscard]] static bool is_key_pressed(key::keycode key) noexcept;
    [[nodiscard]] static bool is_mouse_pressed(key::mouse button) noexcept;
    [[nodiscard]] static vec2 get_mouse_pos() noexcept;
    [[nodiscard]] static f32 get_horizontal() noexcept;
    [[nodiscard]] static f32 get_vertical() noexcept;

  private:
    static ptr<window> win;
};
} // namespace lemon
