#include <lemon/window/window.h>

#include <lemon/core/game.h>
#include <lemon/window/key_codes.h>

#include <lemon/window/window_events.h>

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

    // glfwSetWindowUserPointer(_handle, (void*)&eventDispatcher);
    setup_callbacks();
    event_handler::subscribe(
        string_id("WindowSize"),
        [this](event_base* e) {
            WindowSize* s = (WindowSize*)e;
            _width        = s->width;
            _height       = s->height;
            LOG_MESSAGE("Window resize: %dx%d", _width, _height);
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
        event_handler::
            fire_event(string_id("KeyPressed"),
                       new KeyPressed(
                           key::keycode(k), scancode,
                           key::action(action), key::keymod(mods)));
    });
    glfwSetMouseButtonCallback(_handle, [](GLFWwindow* w, int button, int action, int mods) {
        event_handler ::fire_event(string_id("MouseButtonPressed"),
                                   new MouseButtonPressed(
                                       key::mouse(button),
                                       key::action(action), key::keymod(mods)));
    });
    glfwSetScrollCallback(_handle, [](GLFWwindow* w, double xoffset, double yoffset) {
        event_handler ::fire_event(string_id("MouseScroll"),
                                   new MouseScroll(xoffset, yoffset));
    });
    glfwSetWindowCloseCallback(_handle, [](GLFWwindow* w) {
        event_handler ::fire_event(string_id("WindowClose"),
                                   new WindowClose);
    });
    glfwSetWindowSizeCallback(_handle, [](GLFWwindow* w, int width, int height) {
        event_handler ::fire_event(string_id("WindowSize"),
                                   new WindowSize(width, height));
    });
    glfwSetFramebufferSizeCallback(_handle, [](GLFWwindow* w, int width, int height) {
        event_handler ::fire_event(string_id("FramebufferSize"),
                                   new FramebufferSize(width, height));
    });
    glfwSetWindowContentScaleCallback(_handle, [](GLFWwindow* w, float xscale, float yscale) {
        event_handler ::fire_event(string_id("WindowContentScale"),
                                   new WindowContentScale(xscale, yscale));
    });
    glfwSetWindowPosCallback(_handle, [](GLFWwindow* w, int xpos, int ypos) {
        event_handler ::fire_event(string_id("WindowPos"),
                                   new WindowPos(xpos, ypos));
    });
    glfwSetWindowIconifyCallback(_handle, [](GLFWwindow* w, int iconified) {
        event_handler ::fire_event(string_id("WindowIconify"),
                                   new WindowIconify(iconified));
    });
    glfwSetWindowMaximizeCallback(_handle, [](GLFWwindow* w, int maximized) {
        event_handler ::fire_event(string_id("WindowMaximize"),
                                   new WindowMaximize(maximized));
    });
    glfwSetWindowFocusCallback(_handle, [](GLFWwindow* w, int focused) {
        event_handler ::fire_event(string_id("WindowFocused"),
                                   new WindowFocused(focused));
    });
    glfwSetWindowRefreshCallback(_handle, [](GLFWwindow* w) {
        event_handler ::fire_event(string_id("WindowRefresh"),
                                   new WindowRefresh);
    });
}
}