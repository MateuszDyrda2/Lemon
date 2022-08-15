#pragma once

#include "window_events.h"

#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/vec2.h>

#include <string>

namespace lemon {
class LEMON_API window
{
  public:
    using native_handle_t = void*;

  public:
    window(const std::string& name, const ivec2& size);
    ~window();

    auto get_size() const { return size; }
    auto get_handle() const { return handle; }
    const auto& get_name() const { return name; }

    bool update();

  private:
    std::string name;
    native_handle_t handle;
    ivec2 size;
};
} // namespace lemon
