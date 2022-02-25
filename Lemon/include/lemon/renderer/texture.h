#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/game/object.h>

#include <glm/vec2.hpp>
#include <vector>

namespace lemon {
class LEMON_PUBLIC texture : public object
{
  public:
    texture(string_id name, const std::string& path);
    texture(string_id name, const std::vector<byte>& buffer);
    ~texture();
    texture(const texture&) = delete;
    texture& operator=(const texture&) = delete;
    texture(texture&& other) noexcept;
    texture& operator=(texture&& other) noexcept;

    const glm::ivec2& get_size() const { return size; }
    void bind() const;
    void unbind() const;

  private:
    u32 handle;
    glm::ivec2 size;
    size_type nrOfChannels;
};
} // namespace lemon
