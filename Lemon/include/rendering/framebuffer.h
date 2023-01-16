/** @file frambuffer.h
 * @brief File with the framebuffer definition
 */
#pragma once

#include <core/lemon_types.h>
#include <core/math/vec2.h>

#include <glad/glad.h>

namespace lemon {
/** Framebuffer class */
class framebuffer
{
  public:
    /** @brief Creates a framebuffer with a given size
     * @param size frambuffer size
     */
    framebuffer(const ivec2& size);
    ~framebuffer();
    /** @brief Binds the framebuffer */
    void bind();
    /** @brief Unbinds the framebuffer */
    void unbind();
    /** @brief Changes frambuffer size
     * @param newSize new frambuffer size
     */
    void resize(const ivec2& newSize);
    /** @return frambuffer texture */
    u32 get_texture() const { return texture; }

  private:
    u32 handle;
    u32 texture;
    ivec2 size;
};
} // namespace lemon
