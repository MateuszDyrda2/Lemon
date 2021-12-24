#pragma once

#include <glad/glad.h>

#include <river/core/basic_types.h>
#include <river/event_system/event_handler.h>

namespace river {
class window;
class renderer
{
  public:
    renderer(window* w);
    ~renderer();

    void render();

  private:
    window* _window;
    unsubscriber<size_type, size_type> framebufferSize;
};
} // namespace river