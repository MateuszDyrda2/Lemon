#pragma once

#include <glm/vec2.hpp>
#include <river/core/basic_types.h>
#include <river/game/object.h>

namespace river {
class texture : public object
{
  public:
    texture(string_id name, const std::string& path);
    ~texture();
    texture(const texture&) = delete;
    texture& operator=(const texture&) = delete;
    texture(texture&& other) noexcept;
    texture& operator=(texture&& other) noexcept;

    const glm::ivec2& get_size() const { return size; }
    void bind();
    void unbind();

  private:
    u32 handle;
    glm::ivec2 size;
    size_type nrOfChannels;
};
} // namespace river
