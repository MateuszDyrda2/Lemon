#pragma once

#include "rendering_context.h"

#include <lemon/assets/resource.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

#include <vector>

namespace lemon {
class LEMON_PUBLIC texture : public resource
{
  public:
    texture(string_id name, const std::string& path);
    texture(string_id name, const std::vector<byte>& buffer);
    virtual ~texture();
    texture(texture&& other) noexcept;
    texture& operator=(texture&& other) noexcept;

    const ivec2& get_size() const { return size; }
    void bind() const;
    void unbind() const;

    static string_id get_mocked() { return string_id("mock_texture"); }

  private:
    u32 handle;
    ivec2 size;
    size_type nrOfChannels;
};
} // namespace lemon
