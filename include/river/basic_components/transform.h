#pragma once

#include <river/game/component.h>

#include <glm/vec2.hpp>
#include <memory>
#include <vector>

namespace river {
class transform : public component
{
  public:
    transform(const std::string& name);
    transform(string_id id);

    transform* get_child(size_type index);
    transform* get_child(const std::string& name);
    transform* get_parent();

    void set_parent(transform* parent);
    void rotate(const glm::vec2& axis, float angle);
    void rotate(const glm::vec2& euler);
    void rotate(float x, float y);
    void translate(const glm::vec2& translation);
    glm::vec2 get_world_position() const;
    glm::vec2 get_world_rotation() const;
    glm::vec2 get_world_scale() const;

    glm::vec2& get_position() { return position; }
    glm::vec2& get_rotation() { return rotation; }
    glm::vec2& get_scale() { return scale; }

    const glm::vec2& get_position() const { return position; }
    const glm::vec2& get_rotation() const { return rotation; }
    const glm::vec2& get_scale() const { return scale; }

  private:
    glm::vec2 position, rotation, scale;
    transform* parent;
    std::vector<transform*> children;
};
} // namespace river
