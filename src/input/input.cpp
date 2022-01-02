#include <river/input/input.h>

#include <river/event_system/event_handler.h>
#include <river/window/window.h>

namespace river {
input::input(ptr<window> win):
    _window(win) { }
bool input::is_key_pressed(key::keycode key) const
{
    return glfwGetKey(nullptr, key) & (GLFW_PRESS | GLFW_REPEAT);
}
bool input::is_mouse_pressed(key::mouse button) const
{
    return glfwGetMouseButton(nullptr, button) == GLFW_PRESS;
}
glm::vec2 input::get_mouse_pos() const
{
    double posx, posy;
    glfwGetCursorPos(nullptr, &posx, &posy);
    return glm::vec2(f32(posx), f32(posy));
}
} // namespace river
