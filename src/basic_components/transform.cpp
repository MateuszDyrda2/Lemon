#include <algorithm>
#include <glm/gtx/transform.hpp>
#include <river/basic_components/transform.h>

namespace river {
transform::transform(const std::string& name):
    component(name)
{
}
transform* transform::get_child(size_type index)
{
    return children.at(index);
}
transform* transform::get_child(const std::string& name)
{
    string_id id(name);
    auto res = std::find_if(children.begin(), children.end(),
                            [&](const std::shared_ptr<transform>& ch) {
                                return ch->get_id() == id;
                            });
    return res == children.end() ? nullptr : *res;
}
transform* transform::get_parent()
{
    return parent;
}
void transform::set_parent(transform* parent)
{
    this->parent = parent;
    parent->children.emplace_back(this);
}
void transform::rotate(const glm::vec2& axis, float angle)
{
    rotation += angle * axis;
}
void transform::rotate(const glm::vec2& euler)
{
    rotation += euler;
}
void transform::rotate(float x, float y)
{
    rotation += glm::vec2(x, y);
}
void transform::translate(const glm::vec2& translation)
{
    position += translation;
}
glm::vec2 transform::get_world_position() const
{
    return parent ? parent->get_world_position() + position : position;
}
glm::vec2 transform::get_world_rotation() const
{
    return parent ? parent->get_world_rotation() + rotation : rotation;
}
glm::vec2 transform::get_world_scale() const
{
    return parent ? parent->get_world_scale() + scale : scale;
}

} // namespace river