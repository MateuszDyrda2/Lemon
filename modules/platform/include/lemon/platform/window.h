#pragma once

#include "window_events.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

#include <string>

namespace lemon {
class LEMON_PUBLIC window
{
  public:
    using native_handle_t = void*;

  public:
    window(size_type width, size_type height);
    ~window();

    bool end_frame();
    ivec2 get_size() const { return size; }

    native_handle_t get_handle() { return (void*)_handle; }

  private:
    std::string _name;
    ivec2 size;
    native_handle_t _handle;

  public:
    static event onKeyPressed;
    static event onMouseButtonPressed;
    static event onMouseScroll;
    static event onWindowClose;
    static event onWindowSize;
    static event onFramebufferSize;
    static event onWindowContentScale;
    static event onWindowPos;
    static event onWindowIconify;
    static event onWindowMaximize;
    static event onWindowFocused;
    static event onWindowRefresh;
};
}