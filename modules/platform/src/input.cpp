#include <lemon/platform/systems/input_system.h>

#include <lemon/core/service_registry.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace lemon {
using namespace std;
input_system::input_system(service_registry& globalRegistry):
    win(globalRegistry.get_service<window>())
{
    auto _handle = (GLFWwindow*)win->get_handle();
    glfwSetWindowUserPointer(_handle, this);
    glfwSetKeyCallback(_handle, [](GLFWwindow* w, int k, int scancode, int action, int mods) {
        auto sys        = (input_system*)glfwGetWindowUserPointer(w);
        key_state value = 0.f;
        if(action == GLFW_PRESS || action == GLFW_REPEAT)
            value = 1.f;

        sys->devices.keyboardState[key::keycode(k)] = value;
    });
    glfwSetMouseButtonCallback(_handle, [](GLFWwindow* w, int button, int action, int mods) {
        auto sys        = (input_system*)glfwGetWindowUserPointer(w);
        key_state value = 0.f;
        if(action == GLFW_PRESS || action == GLFW_REPEAT)
            value = 1.f;

        sys->devices.mouseState[key::keycode(key::MouseFirst + button)] = value;
    });
    glfwSetCursorPosCallback(_handle, [](GLFWwindow* w, double xpos, double ypos) {
        auto sys    = (input_system*)glfwGetWindowUserPointer(w);
        key_state x = key_state(xpos), y = key_state(ypos);

        sys->devices.mouseState[key::keycode::mouse_x] = x;
        sys->devices.mouseState[key::keycode::mouse_y] = y;
    });
    // enumerate all potential connected gamepads
    for(size_type i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; ++i)
    {
        glfwSetJoystickUserPointer(i, this);
        if(glfwJoystickIsGamepad(i) == GLFW_TRUE)
        {
            devices.gamepadState.emplace(make_pair(i, unordered_map<key::keycode, key_state>()));
        }
    }
    // set gamepad callback ( called when a gamepad is connected / disconnected )
    glfwSetJoystickCallback([](int jid, int event) {

    });
}
input_system::~input_system()
{
}
void input_system::update(entity_registry& registry)
{
    GLFWgamepadstate state;
    for(auto&& [jid, gamepad] : devices.gamepadState)
    {
        glfwGetGamepadState(jid, &state);
        size_type j = GAMEPAD_FIRST, i;

        for(i = GLFW_GAMEPAD_BUTTON_A; i < GLFW_GAMEPAD_BUTTON_LAST; ++i, ++j)
        {
            auto bs = state.buttons[i];
            if(bs != keyStates[j])
            {
                if(bs == GLFW_PRESS)
                {
                    // button pressed event
                    keyStates[j] = 1.f;
                }
                else
                {
                    // button released event
                    keyStates[j] = 0.f;
                }
            }
        }
        for(i = GLFW_GAMEPAD_AXIS_LEFT_X; i < GLFW_GAMEPAD_AXIS_LAST; ++i, ++j)
        {
            auto bs      = state.axes[i];
            keyStates[j] = bs;
            if(bs != 0.0f)
            {
                // axis moved
            }
        }
    }
}
void input_system::map2event(key::keycode kc, string_id eventName)
{
}
void input_system::gather_input()
{
    for(auto& device : inputDevices)
    {
    }
}
} // namespace lemon