#pragma once

#include <glm/vec2.hpp>
#include <river/core/basic_types.h>
#include <river/game/object.h>

namespace river {
class texture : public object
{
  public:
    texture(const std::string& name, const std::string& path);
    ~texture();

    const glm::vec2& get_size() const { return size; }
    void bind();

  private:
    u32 handle;
    glm::ivec2 size;
    size_type nrOfChannels;
};
} // namespace river
