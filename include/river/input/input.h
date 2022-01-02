#pragma once

#include "key_codes.h"

#include <river/core/basic_types.h>
#include <river/service/service.h>

#include <glm/vec2.hpp>

namespace river {
class window_base;
class input : public service
{
  public:
    input();
    ~input() = default;
    [[nodiscard]] bool is_key_pressed(key::keycode key) const;
    [[nodiscard]] bool is_mouse_pressed(key::mouse button) const;
    [[nodiscard]] glm::vec2 get_mouse_pos() const;

  private:
    ptr<window_base> _window;
};
} // namespace river
