#pragma once

#include <river/game/component.h>

#include <glm/vec3.hpp>
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
    void rotate(const glm::vec3& axis, float angle);
    void rotate(const glm::vec3& euler);
    void rotate(float x, float y);
    void translate(const glm::vec3& translation);
    glm::vec3 get_world_position() const;
    glm::vec3 get_world_rotation() const;
    glm::vec3 get_world_scale() const;

    glm::vec3& get_position() { return position; }
    glm::vec3& get_rotation() { return rotation; }
    glm::vec3& get_scale() { return scale; }

    const glm::vec3& get_position() const { return position; }
    const glm::vec3& get_rotation() const { return rotation; }
    const glm::vec3& get_scale() const { return scale; }

  private:
    glm::vec3 position, rotation, scale;
    transform* parent;
    std::vector<transform*> children;
};
} // namespace river
