#include <lemon/input/input.h>

#include <lemon/event_system/event_handler.h>
#include <lemon/service/services.h>
#include <lemon/window/window.h>

namespace lemon {
input::input():
    _window(services::get<window_base>()) { }
bool input::is_key_pressed(key::keycode key) const
{
    return glfwGetKey(
               (GLFWwindow*)_window->get_handle(), key)
           & (GLFW_PRESS | GLFW_REPEAT);
}
bool input::is_mouse_pressed(key::mouse button) const
{
    return glfwGetMouseButton(
               (GLFWwindow*)_window->get_handle(), button)
           == GLFW_PRESS;
}
glm::vec2 input::get_mouse_pos() const
{
    double posx, posy;
    glfwGetCursorPos(
        (GLFWwindow*)_window->get_handle(), &posx, &posy);
    return glm::vec2(f32(posx), f32(posy));
}
} // namespace lemon
