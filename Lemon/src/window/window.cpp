#include <lemon/window/window.h>

#include <lemon/game.h>
#include <lemon/input/key_codes.h>

namespace lemon {
window::window(size_type width, size_type height):
    window_base(width, height), _name(game::get_settings().gameName)
{
    if(!glfwInit())
    {
        LOG_FATAL("GLFW initialization failure");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(!(_handle = glfwCreateWindow(width, height, _name.c_str(), NULL, NULL)))
    {
        LOG_FATAL("Window or OpenGL context creation failed");
    }
    glfwMakeContextCurrent(_handle);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(_handle, (void*)&eventDispatcher);
    setup_callbacks();
    windowResize = listener<int, int>(
        string_id("WindowSize"),
        [this](int width, int height) {
            LOG_MESSAGE("Window resize: %dx%d", width, height);
            _width  = width;
            _height = height;
        });

    glfwSetErrorCallback([](int /* error */, const char* description) {
        LOG_ERROR("GLFWError: %s", description);
    });
    LOG_MESSAGE("Window created %dx%d", width, height);
}
window::~window()
{
    glfwDestroyWindow(_handle);
    glfwTerminate();
    LOG_MESSAGE("Window destroyed");
}
bool window::end_frame()
{
    glfwSwapBuffers(_handle);
    glfwPollEvents();
    return !!glfwWindowShouldClose(_handle);
}
void window::setup_callbacks()
{
    glfwSetKeyCallback(_handle, [](GLFWwindow* w, int k, int scancode, int action, int mods) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("KeyPressed"), key::keycode(k), scancode, key::action(action), key::keymod(mods));
    });
    glfwSetMouseButtonCallback(_handle, [](GLFWwindow* w, int button, int action, int mods) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("MouseButtonPressed"), key::mouse(button),
                   key::action(action), key::keymod(mods));
    });
    glfwSetScrollCallback(_handle, [](GLFWwindow* w, double xoffset, double yoffset) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("MouseScroll"), xoffset, yoffset);
    });
    glfwSetWindowCloseCallback(_handle, [](GLFWwindow* w) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("WindowClose"));
    });
    glfwSetWindowSizeCallback(_handle, [](GLFWwindow* w, int width, int height) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("WindowSize"), width, height);
    });
    glfwSetFramebufferSizeCallback(_handle, [](GLFWwindow* w, int width, int height) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("FramebufferSize"), width, height);
    });
    glfwSetWindowContentScaleCallback(_handle, [](GLFWwindow* w, float xscale, float yscale) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("WindowContentScale"), xscale, yscale);
    });
    glfwSetWindowPosCallback(_handle, [](GLFWwindow* w, int xpos, int ypos) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("WindowPos"), xpos, ypos);
    });
    glfwSetWindowIconifyCallback(_handle, [](GLFWwindow* w, int iconified) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("WindowIconify"), iconified);
    });
    glfwSetWindowMaximizeCallback(_handle, [](GLFWwindow* w, int maximized) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("WindowMaximize"), maximized);
    });
    glfwSetWindowFocusCallback(_handle, [](GLFWwindow* w, int focused) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("WindowFocused"), focused);
    });
    glfwSetWindowRefreshCallback(_handle, [](GLFWwindow* w) {
        ((dispatcher*)glfwGetWindowUserPointer(w))
            ->send(string_id("WindowRefresh"));
    });
}
}