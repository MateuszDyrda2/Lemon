#include <lemon/window/input.h>

#include <lemon/window/window.h>

namespace lemon {
window_base* input::_window = nullptr;
input::input(ptr<window_base> window)
{
    _window = window;
    LOG_MESSAGE("Input manager created");
}
input::~input()
{
    LOG_MESSAGE("Input manager destroyed");
}
bool input::is_key_pressed(key::keycode key)
{
    return glfwGetKey(
               (GLFWwindow*)_window->get_handle(), key)
           & (GLFW_PRESS | GLFW_REPEAT);
}
bool input::is_mouse_pressed(key::mouse button)
{
    return glfwGetMouseButton(
               (GLFWwindow*)_window->get_handle(), button)
           == GLFW_PRESS;
}
glm::vec2 input::get_mouse_pos()
{
    double posx, posy;
    glfwGetCursorPos(
        (GLFWwindow*)_window->get_handle(), &posx, &posy);
    return glm::vec2(f32(posx), f32(posy));
}
} // namespace lemon
