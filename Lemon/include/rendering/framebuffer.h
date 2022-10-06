#pragma once

#include <core/lemon_types.h>
#include <core/math/vec2.h>

#include <glad/glad.h>

namespace lemon {
class framebuffer
{
  public:
    framebuffer(const ivec2& size);
    ~framebuffer();
    void bind();
    void unbind();
    void resize(const ivec2& newSize);
    u32 get_texture() const { return texture; }

  private:
    u32 handle;
    u32 texture;
    ivec2 size;
};
} // namespace lemon