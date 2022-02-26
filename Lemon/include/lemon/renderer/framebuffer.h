#pragma once

#include <lemon/core/basic_types.h>

#include <glad/glad.h>
#include <glm/vec2.hpp>

namespace lemon {
class LEMON_PUBLIC framebuffer
{
  public:
    framebuffer(const glm::ivec2& size);
    ~framebuffer();
    void bind();
    void unbind();
    void resize(const glm::ivec2& newSize);
    u32 get_texture() const { return texture; }

  private:
    u32 handle;
    u32 texture;
    glm::ivec2 size;
};
} // namespace lemon