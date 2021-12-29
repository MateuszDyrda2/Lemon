#pragma once

#include "shader.h"
#include "texture.h"
#include <river/basic_components/transform.h>
#include <river/core/basic_types.h>
#include <river/event_system/event_handler.h>

#include <glad/glad.h>
#include <memory>

namespace river {
class window;
class scene;
class renderer
{
  public:
    renderer(ptr<window> w);
    ~renderer();

    void render(ptr<scene> currentScene);
    void draw_sprite(texture& tex, transform& trans);

  private:
    unsubscriber<size_type, size_type> framebufferSize;

  private:
    void init_renderer_data();
    unsigned int VAO;
    std::unique_ptr<shader> textureShader;
};
} // namespace river