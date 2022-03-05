#pragma once

#include "rendering_context.h"

#include <lemon/assets/resource.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/color.h>
#include <lemon/core/math/vec2.h>

#include <vector>

namespace lemon {
class LEMON_PUBLIC texture : public resource
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
    texture(string_id name, const std::string& path);
    texture(string_id name, const std::vector<byte>& buffer);
    texture(string_id name, const std::string& path, wrap wrapping, filter filtering);
    texture(string_id name, const ivec2& size, const color& c);
    texture(string_id name, const ivec2& size, const color& c, wrap wrapping, filter filtering);
    virtual ~texture();
    texture(const texture&) = delete;
    texture& operator=(const texture&) = delete;
    texture(texture&& other) noexcept;
    texture& operator=(texture&& other) noexcept;

    const ivec2& get_size() const { return size; }
    wrap get_wraping() const { return wrapping; }
    filter get_filtering() const { return filtering; }

    void bind() const;
    void unbind() const;

    static string_id get_mocked() { return string_id("mock_texture"); }

  private:
    u32 handle;
    ivec2 size;
    size_type nrOfChannels;
    wrap wrapping;
    filter filtering;
};
} // namespace lemon
