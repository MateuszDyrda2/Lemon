#include <platform/input.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <array>
#include <map>

namespace lemon {
class input::impl
{
    struct input_action
    {
        struct keyboard_axis_action
        {
            keycode first;
            keycode second;
        };
        struct keyboard_action
        {
            keycode key;
            key_action action;
        };
        struct mouse_action
        {
            mouse key;
            key_action action;
        };
        struct gamepad_action
        {
            gamepad key;
            key_action action;
        };
        union
        {
            mouse_axis mouseAxis;
            gamepad_axis gamepadAxis;
            keyboard_axis_action keyboardAxis;
            keyboard_action keyAction;
            mouse_action mouseAction;
            gamepad_action gamepadAction;
        };
        device dev;
        constexpr bool operator==(const input_action& other) const noexcept
        {
            if(dev != other.dev) return false;
            switch(dev)
            {
            case device::keyboard:
                return keyAction.action == other.keyAction.action
                       && keyAction.key == other.keyAction.key;
            case device::keyboard_axis:
                return keyboardAxis.first == other.keyboardAxis.first
                       && keyboardAxis.second == other.keyboardAxis.second;
            case device::mouse:
                return mouseAction.action == other.mouseAction.action
                       && mouseAction.key == other.mouseAction.key;
            case device::mouse_axis:
                return mouseAxis == other.mouseAxis;
            case device::gamepad:
                return gamepadAction.action == other.gamepadAction.action
                       && gamepadAction.key == other.gamepadAction.key;
            case device::gamepad_axis:
                return gamepadAxis == other.gamepadAxis;

            default:
                return false;
            }
        }
    };
    struct in_hash
    {
        std::size_t operator()(const input_action& act) const
        {
            switch(act.dev)
            {
            case device::keyboard:
                return std::hash<u64>{}(u64(act.keyAction.key) << 32 | u64(act.keyAction.action));
            case device::keyboard_axis:
                return std::hash<u64>{}(u64(act.keyboardAxis.first) << 32 | u64(act.keyboardAxis.second));
            case device::mouse:
                return std::hash<u64>{}(u64(act.mouseAction.key) << 32 | u64(act.mouseAction.action));
            case device::mouse_axis:
                return std::hash<u64>{}(u64(act.mouseAxis));
            case device::gamepad:
                return std::hash<u64>{}(u64(act.gamepadAction.key) << 32 | u64(act.gamepadAction.action));
            case device::gamepad_axis:
                return std::hash<u64>{}(u64(act.gamepadAxis));

            default:
                return 0ULL;
            }
        }
    };

    using action_id = hash_str;

  public:
    impl(window& wnd, event_queue& queue):
        jid{ -1 }, queue(queue)
    {
        glfwSetWindowUserPointer(reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
                                 reinterpret_cast<void*>(this));
        glfwSetKeyCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* wnd, int key, int /*scancode*/, int action, int /*mods*/) {
                auto in            = reinterpret_cast<impl*>(glfwGetWindowUserPointer(wnd));
                in->keyStates[key] = key_action(1 << action);
            });
        glfwSetMouseButtonCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* wnd, int button, int action, int /*mods*/) {
                auto in                 = reinterpret_cast<impl*>(glfwGetWindowUserPointer(wnd));
                in->mouseStates[button] = key_action(1 << action);
            });
        glfwSetCursorPosCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* wnd, f64 xpos, f64 ypos) {
                auto in         = reinterpret_cast<impl*>(glfwGetWindowUserPointer(wnd));
                in->mousePos[0] = f32(xpos);
                in->mousePos[1] = f32(ypos);
            });
        initialize_events(wnd);

        for(int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; ++i)
        {
            glfwSetJoystickUserPointer(i, reinterpret_cast<void*>(this));
            if(glfwJoystickIsGamepad(i) == GLFW_TRUE)
            {
                jid = std::size_t(i);
            }
        }
        glfwSetJoystickCallback([](int jid, int event) {
            auto in = reinterpret_cast<impl*>(glfwGetJoystickUserPointer(jid));
            if(event == GLFW_CONNECTED)
            {
                in->jid = jid;
            }
            else if(event == GLFW_DISCONNECTED)
            {
                in->jid = -1;
            }
        });
    }
    void initialize_events(window& wnd)
    {
        glfwSetWindowCloseCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* w) {
                auto in = reinterpret_cast<impl*>(glfwGetWindowUserPointer(w));
                in->queue["WindowClose"_hs].fire(new window_close);
            });

        glfwSetWindowSizeCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* w, int width, int height) {
                auto in = reinterpret_cast<impl*>(glfwGetWindowUserPointer(w));
                in->queue["WindowSize"_hs].fire(new window_size(width, height));
            });

        glfwSetFramebufferSizeCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* w, int width, int height) {
                auto in = reinterpret_cast<impl*>(glfwGetWindowUserPointer(w));
                in->queue["FramebufferSize"_hs].fire(new framebuffer_size(width, height));
            });

        glfwSetWindowContentScaleCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* w, f32 xscale, f32 yscale) {
                auto in = reinterpret_cast<impl*>(glfwGetWindowUserPointer(w));
                in->queue["WindowContentScale"_hs].fire(new window_content_scale(xscale, yscale));
            });

        glfwSetWindowPosCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* w, int xpos, int ypos) {
                auto in = reinterpret_cast<impl*>(glfwGetWindowUserPointer(w));
                in->queue["WindowPos"_hs].fire(new window_pos(xpos, ypos));
            });

        glfwSetWindowIconifyCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* w, int iconified) {
                auto in = reinterpret_cast<impl*>(glfwGetWindowUserPointer(w));
                in->queue["WindowIconify"_hs].fire(new window_iconify(iconified));
            });

        glfwSetWindowMaximizeCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* w, int maximized) {
                auto in = reinterpret_cast<impl*>(glfwGetWindowUserPointer(w));
                in->queue["WindowMaximize"_hs].fire(new window_maximize(maximized));
            });

        glfwSetWindowFocusCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* w, int focused) {
                auto in = reinterpret_cast<impl*>(glfwGetWindowUserPointer(w));
                in->queue["WindowFocus"_hs].fire(new window_focus(focused));
            });

        glfwSetWindowRefreshCallback(
            reinterpret_cast<GLFWwindow*>(wnd.get_handle()),
            [](GLFWwindow* w) {
                auto in = reinterpret_cast<impl*>(glfwGetWindowUserPointer(w));
                in->queue["WindowFocus"_hs].fire(new window_refresh);
            });
    }
    void update()
    {
        if(jid != -1)
        {
            GLFWgamepadstate newState;
            glfwGetGamepadState(jid, &newState);
            for(std::size_t i = 0; i < GAMEPAD_COUNT; ++i)
            {
                if(gamepadStates[i] != key_action(newState.buttons[i]))
                {
                    gamepadStates[i] = key_action(1 << newState.buttons[i]);
                }
                else if(newState.buttons[i] == GLFW_RELEASE)
                {
                    gamepadStates[i] = key_action::up;
                }
            }
            for(std::size_t i = 0; i < GAMEPAD_AXIS_COUNT; ++i)
            {
                gamepadAxisStates[i] = f32(newState.axes[i]);
            }
        }
        for(auto&& [inp, acts] : actions)
        {
            switch(inp.dev)
            {
            case device::keyboard:
            {
                auto&& [key, action] = inp.keyAction;
                if(i32(action) & i32(keyStates[int(key)]))
                    queue[acts].fire(new input_event);
            }
            break;
            case device::keyboard_axis:
            {
                auto&& [first, second] = inp.keyboardAxis;
                auto f                 = f32(keyStates[i32(first)] == key_action::pressed
                                             || keyStates[i32(first)] == key_action::down);
                auto s                 = f32(keyStates[i32(second)] == key_action::pressed
                                             || keyStates[i32(second)] == key_action::down);
                queue[acts].fire(new axis_event(s - f));
            }
            break;
            case device::mouse:
            {
                auto&& [key, action] = inp.mouseAction;
                if(i32(action) & i32(mouseStates[i32(key) - i32(MOUSE_FIRST)]))
                    queue[acts].fire(new input_event);
            }
            break;
            case device::mouse_axis:
            {
                queue[acts].fire(new axis_event(mousePos[i32(inp.mouseAxis) - i32(MOUSE_AXIS_FIRST)]));
            }
            break;
            case device::gamepad:
            {
                auto&& [key, action] = inp.gamepadAction;
                if(i32(action) & i32(keyStates[i32(key) - i32(GAMEPAD_FIRST)]))
                    queue[acts].fire(new input_event);
            }
            break;
            case device::gamepad_axis:
            {
                queue[acts].fire(new axis_event(gamepadAxisStates[i32(inp.gamepadAxis) - i32(GAMEPAD_AXIS_FIRST)]));
            }
            break;
            default:
                break;
            }
        }
        for(auto& k : keyStates)
        {
            if(k == key_action::pressed) k = key_action::down;
            if(k == key_action::released) k = key_action::up;
        }
        mouseStates.fill(key_action::up);
    }
    void add_key(keycode key, key_action action, hash_str actionId)
    {
        const auto in = input_action{
            .keyAction = { key, action },
            .dev       = device::keyboard
        };
        actions[in] = actionId;
    }
    void add_key(mouse button, key_action action, hash_str actionId)
    {
        const auto in = input_action{
            .mouseAction = { button, action },
            .dev         = device::mouse
        };
        actions[in] = actionId;
    }
    void add_key(gamepad button, key_action action, hash_str actionId)
    {
        const auto in = input_action{
            .gamepadAction = { button, action },
            .dev           = device::gamepad
        };
        actions[in] = actionId;
    }
    void add_axis(mouse_axis mouse, hash_str actionId)
    {
        const auto in = input_action{
            .mouseAxis = mouse,
            .dev       = device::mouse_axis
        };
        actions[in] = actionId;
    }
    void add_axis(gamepad_axis gamepad, hash_str actionId)
    {
        const auto in = input_action{
            .gamepadAxis = gamepad,
            .dev         = device::gamepad_axis
        };
        actions[in] = actionId;
    }
    void add_axis(pair_keycodes axis, hash_str actionId)
    {
        const auto in = input_action{
            .keyboardAxis = { axis.first, axis.second },
            .dev          = device::keyboard_axis
        };
        actions[in] = actionId;
    }

  private:
    std::array<key_action, KEYBOARD_COUNT> keyStates;
    std::array<key_action, MOUSE_COUNT> mouseStates;
    std::array<key_action, GAMEPAD_COUNT> gamepadStates;
    std::array<f32, MOUSE_AXIS_COUNT> mousePos;
    std::array<f32, GAMEPAD_AXIS_COUNT> gamepadAxisStates;

    std::unordered_map<input_action, action_id, in_hash> actions;

    i32 jid;
    event_queue& queue;
};

input::input(window& wnd, event_queue& queue):
    pImpl{ std::make_unique<impl>(wnd, queue) }
{
    logger::info("Input created");
}
input::~input()
{
    logger::info("Input destroyed");
}
void input::update()
{
    pImpl->update();
}
void input::add_key(keycode key, key_action action, hash_str actionId)
{
    pImpl->add_key(key, action, actionId);
}
void input::add_key(mouse button, key_action action, hash_str actionId)
{
    pImpl->add_key(button, action, actionId);
}
void input::add_key(gamepad button, key_action action, hash_str actionId)
{
    pImpl->add_key(button, action, actionId);
}
void input::add_axis(mouse_axis mouse, hash_str actionId)
{
    pImpl->add_axis(mouse, actionId);
}
void input::add_axis(gamepad_axis gamepad, hash_str actionId)
{
    pImpl->add_axis(gamepad, actionId);
}
void input::add_axis(pair_keycodes axis, hash_str actionId)
{
    pImpl->add_axis(axis, actionId);
}
} // namespace lemon
