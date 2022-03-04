#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

#include <glad/glad.h>

namespace lemon {
class LEMON_PUBLIC framebuffer
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
