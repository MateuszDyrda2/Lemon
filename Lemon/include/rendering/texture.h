/** @file texture.h
 * @brief File with a texture definition
 */
#pragma once

#include "rendering_context.h"

#include <assets/resource.h>
#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/color.h>
#include <core/math/vec2.h>

#include <vector>

namespace lemon {
/** texture representation */
class LEMON_API texture : public resource
{
  public:
    /** Wrapping type */
    enum class wrap
    {
        repeat         = GL_REPEAT,
        mirroredRepeat = GL_MIRRORED_REPEAT,
        clampToEdge    = GL_CLAMP_TO_EDGE,
        clampToBorder  = GL_CLAMP_TO_BORDER
    };
    /** Filter type */
    enum class filter
    {
        nearest = GL_NEAREST,
        linear  = GL_LINEAR
    };

  public:
    /** @brief Creates a texture
     * @param nameid id of the texture
     * @param path path to the texture resource
     */
    texture(hashstr nameid, const std::string& path);
    /** @brief Creates a texture
     * @param nameid id of the texture
     * @param buffer texture data
     */
    texture(hashstr nameid, const std::vector<u8>& buffer);
    /** @brief Creates a texture
     * @param nameid id of the texture
     * @param path path to the texture resource
     * @param wrapping wrap type
     * @param filtering filter type
     */
    texture(hashstr nameid, const std::string& path, wrap wrapping, filter filtering);
    /** @brief Creates a texture
     * @param nameid id of the texture
     * @param size texture size
     * @param c texture color
     */
    texture(hashstr nameid, const ivec2& size, const color& c);
    /** @brief Creates a texture
     * @param nameid id of the texture
     * @param size texture size
     * @param c texture color
     * @param wrapping texture wrap
     * @param filtering texture filter
     */
    texture(hashstr nameid, const ivec2& size, const color& c, wrap wrapping, filter filtering);
    virtual ~texture();
    texture(const texture&)            = delete;
    texture& operator=(const texture&) = delete;
    texture(texture&& other) noexcept;
    texture& operator=(texture&& other) noexcept;
    /** @return texture size */
    const ivec2& get_size() const { return size; }
    /** @return texture wrap */
    wrap get_wraping() const { return wrapping; }
    /** @return texture filter */
    filter get_filtering() const { return filtering; }
    /** @brief Bind texture */
    void bind() const;
    /** @brief Unbind texture */
    void unbind() const;
    /** @brief Get mocked texture id */
    static hashstr get_mocked() { return "mock_texture"_hs; }

  private:
    u32 handle;
    ivec2 size;
    std::size_t nrOfChannels;
    wrap wrapping;
    filter filtering;
};
} // namespace lemon
