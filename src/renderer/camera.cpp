#include <glm/gtx/transform.hpp>
#include <river/renderer/camera.h>

namespace river {
camera::camera(const std::string& name, scene* this_scene):
    entity(name, this_scene)
{
}
camera::camera(string_id id, scene* this_scene):
    entity(id, this_scene)
{
}
void camera::set_viewport(const glm::vec4& newViewport) noexcept
{
    viewport = newViewport;
    recalculate_projection();
}
glm::mat4 camera::get_projection() const noexcept
{
    return projection;
}
glm::mat4 camera::get_view() const noexcept
{
    glm::mat4 view = glm::mat4(1.0f);
    view           = glm::scale(view, get_transform().get_scale());
    view           = glm::rotate(view,
                                 glm::length(get_transform().get_rotation()),
                                 glm::normalize(get_transform().get_rotation()));
    view           = glm::translate(view, get_transform().get_position());

    return view;
}
glm::mat4 camera::get_view_projection() const noexcept
{
    return get_projection() * get_view();
}
} // namespace river
