#pragma once

#include <river/game/component.h>

#include <glm/vec2.hpp>
#include <memory>
#include <vector>

namespace river {
class transform : public component
{
  public:
    transform(std::string const& name);
    transform(string_id id);

    std::shared_ptr<transform> get_child(size_type index);
    std::shared_ptr<transform> get_child(std::string const& name);
    std::shared_ptr<transform> get_parent();

    void set_parent(std::shared_ptr<transform> const& parent);
    void rotate(glm::vec2 const& axis, float angle);
    void rotate(glm::vec2 const& euler);
    void rotate(float x, float y);
    void translate(glm::vec2 const& translation);
    glm::vec2 get_world_position() const;
    glm::vec2 get_world_rotation() const;
    glm::vec2 get_world_scale() const;

    glm::vec2 position, rotation, scale;
    std::shared_ptr<transform> parent;
    std::vector<std::shared_ptr<transform>> children;
};
} // namespace river
