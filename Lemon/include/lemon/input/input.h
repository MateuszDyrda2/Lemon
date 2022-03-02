#pragma once

#include "key_codes.h"

#include <lemon/core/basic_types.h>
#include <lemon/service/service.h>
#include <lemon/window/window_base.h>

#include <glm/vec2.hpp>

namespace lemon {
class LEMON_PUBLIC input
{
  public:
    input(ptr<window_base> window);
    ~input();
    [[nodiscard]] bool is_key_pressed(key::keycode key) const;
    [[nodiscard]] bool is_mouse_pressed(key::mouse button) const;
    [[nodiscard]] glm::vec2 get_mouse_pos() const;

  private:
    ptr<window_base> _window;
};
} // namespace lemon
