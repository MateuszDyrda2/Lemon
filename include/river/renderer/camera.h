#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <river/game/entity.h>

namespace river {
class camera : public entity
{
  public:
    camera(const std::string& name, scene* this_scene);
    camera(string_id id, scene* this_scene);

    const glm::vec4& get_viewport() const noexcept { return viewport; }
    void set_viewport(const glm::vec4& newViewport) noexcept;

    glm::mat4 get_projection() const noexcept;
    glm::mat4 get_view() const noexcept;
    glm::mat4 get_view_projection() const noexcept;

  protected:
    glm::vec4 viewport;
    glm::mat4 projection;
    virtual void recalculate_projection() = 0;
};
} // namespace river