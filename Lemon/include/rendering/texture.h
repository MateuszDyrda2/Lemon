#pragma once

#include "rendering_context.h"

#include <assets/resource.h>
#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/color.h>
#include <core/math/vec2.h>

#include <vector>

namespace lemon {
class LEMON_API texture : public resource
{
  public:
    enum class wrap
    {
        repeat         = GL_REPEAT,
        mirroredRepeat = GL_MIRRORED_REPEAT,
        clampToEdge    = GL_CLAMP_TO_EDGE,
        clampToBorder  = GL_CLAMP_TO_BORDER
    };
    enum class filter
    {
        nearest = GL_NEAREST,
        linear  = GL_LINEAR
    };

  public:
    texture(hashstr name, const std::string& path);
    texture(hashstr name, const std::vector<u8>& buffer);
    texture(hashstr name, const std::string& path, wrap wrapping, filter filtering);
    texture(hashstr name, const ivec2& size, const color& c);
    texture(hashstr name, const ivec2& size, const color& c, wrap wrapping, filter filtering);
    virtual ~texture();
    texture(const texture&)            = delete;
    texture& operator=(const texture&) = delete;
    texture(texture&& other) noexcept;
    texture& operator=(texture&& other) noexcept;

    const ivec2& get_size() const { return size; }
    wrap get_wraping() const { return wrapping; }
    filter get_filtering() const { return filtering; }

    void bind() const;
    void unbind() const;

    static hashstr get_mocked() { return "mock_texture"_hs; }

  private:
    u32 handle;
    ivec2 size;
    std::size_t nrOfChannels;
    wrap wrapping;
    filter filtering;
};
} // namespace lemon
