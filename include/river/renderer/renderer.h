#pragma once

#include "shader.h"
#include <river/core/basic_types.h>
#include <river/event_system/event_handler.h>

#include <glad/glad.h>
#include <memory>

namespace river {
class window;
class renderer
{
  public:
    renderer(window* w);
    ~renderer();

    void render();
    void draw_sprite();

  private:
    window* _window;
    unsubscriber<size_type, size_type> framebufferSize;

  private:
    void init_renderer_data();
    unsigned int VAO;
    std::unique_ptr<shader> textureShader;
};
} // namespace river