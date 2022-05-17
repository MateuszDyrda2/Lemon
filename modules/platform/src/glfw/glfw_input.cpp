#include <lemon/platform/input.h>

#include <lemon/core/logger.h>
#include <lemon/platform/window.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace lemon {
static GLFWgamepadstate gamepadState;
input::input(window& window):
    win(window)
{
    if(glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
    {
        LOG_MESSAGE("Joystick connected %s", glfwGetJoystickName(GLFW_JOYSTICK_1));
    }
}
input::~input()
{ }
void input::update()
{
    glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepadState);
}
bool input::is_key_pressed(key::keycode key) noexcept
{
    return glfwGetKey(
               (GLFWwindow*)win.get_handle(), key)
           & (GLFW_PRESS | GLFW_REPEAT);
}
bool input::is_mouse_pressed(key::mouse button) noexcept
{
    return glfwGetMouseButton(
               (GLFWwindow*)win.get_handle(), button)
           == GLFW_PRESS;
}
bool input::is_gamepad_key_pressed(key::gamepad key) noexcept
{
    return gamepadState.buttons[int(key)] == GLFW_PRESS;
}
f32 input::get_gamepad_axis(key::axis axis) noexcept
{
    return gamepadState.axes[int(axis)];
}
vec2 input::get_mouse_pos() noexcept
{
    double posx, posy;
    glfwGetCursorPos(
        (GLFWwindow*)win.get_handle(), &posx, &posy);
    return vec2(f32(posx), f32(posy));
}
f32 input::get_horizontal() noexcept
{
    return (-1.0f) * is_key_pressed(key::keycode::a)
           + (1.0f) * is_key_pressed(key::keycode::d);
}
f32 input::get_vertical() noexcept
{
    return (-1.0f) * is_key_pressed(key::keycode::s)
           + (1.0f) * is_key_pressed(key::keycode::w);
}
} // namespace lemon
