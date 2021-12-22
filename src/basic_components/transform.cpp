#include <algorithm>
#include <glm/gtx/transform.hpp>
#include <river/basic_components/transform.h>

namespace river {
transform::transform(std::string const& name):
    component(name)
{
}
std::shared_ptr<transform> transform::get_child(size_type index)
{
    return children.at(index);
}
std::shared_ptr<transform> transform::get_child(std::string const& name)
{
    string_id id(name);
    auto res = std::find_if(children.begin(), children.end(),
                            [&](std::shared_ptr<transform> const& ch) {
                                return ch->get_id() == id;
                            });
    return res == children.end() ? std::shared_ptr<transform>() : *res;
}
std::shared_ptr<transform> transform::get_parent()
{
    return parent;
}
void transform::set_parent(std::shared_ptr<transform> const& parent)
{
    this->parent = parent;
    parent->children.emplace_back(this);
}
void transform::rotate(glm::vec2 const& axis, float angle)
{
    rotation += angle * axis;
}
void transform::rotate(glm::vec2 const& euler)
{
    rotation += euler;
}
void transform::rotate(float x, float y)
{
    rotation += glm::vec2(x, y);
}
void transform::translate(glm::vec2 const& translation)
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