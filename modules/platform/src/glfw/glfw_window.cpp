#include <lemon/platform/window.h>

#include <lemon/core/game.h>
#include <lemon/events/event_handler.h>
#include <lemon/platform/key_codes.h>

#include <lemon/platform/window_events.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace lemon {
void setup_callbacks(GLFWwindow* _handle);
window::window(size_type width, size_type height):
    _name(game::get_settings().gameName), size(width, height)
{
    if(!glfwInit())
    {
        LOG_FATAL("GLFW initialization failure");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(!(_handle = (void*)glfwCreateWindow(width, height, _name.c_str(), NULL, NULL)))
    {
        LOG_FATAL("Window or OpenGL context creation failed");
    }
    glfwMakeContextCurrent((GLFWwindow*)_handle);
    glfwSwapInterval(1);

    // glfwSetWindowUserPointer(_handle, (void*)&eventDispatcher);
    setup_callbacks((GLFWwindow*)_handle);
    event_handler::subscribe(
        string_id("WindowSize"),
        [this](event_base* e) {
            WindowSize* s = (WindowSize*)e;
            size          = ivec2(s->width, s->height);
            LOG_MESSAGE("Window resize: %dx%d", size.x, size.y);
        });

    glfwSetErrorCallback([](int /* error */, const char* description) {
        LOG_ERROR("GLFWError: %s", description);
    });
    LOG_MESSAGE("Window created %dx%d", width, height);
}
window::~window()
{
    glfwDestroyWindow((GLFWwindow*)_handle);
    glfwTerminate();
    LOG_MESSAGE("Window destroyed");
}
bool window::end_frame()
{
    glfwSwapBuffers((GLFWwindow*)_handle);
    glfwPollEvents();
    return !!glfwWindowShouldClose((GLFWwindow*)_handle);
}
void setup_callbacks(GLFWwindow* _handle)
{
    glfwSetKeyCallback(_handle, [](GLFWwindow*, int k, int scancode, int action, int mods) {
        event_handler::
            fire_event(string_id("KeyPressed"),
                       new KeyPressed(
                           key::keycode(k), scancode,
                           key::action(action), key::keymod(mods)));
    });
    glfwSetMouseButtonCallback(_handle, [](GLFWwindow*, int button, int action, int mods) {
        event_handler ::fire_event(string_id("MouseButtonPressed"),
                                   new MouseButtonPressed(
                                       key::mouse(button),
                                       key::action(action), key::keymod(mods)));
    });
    glfwSetScrollCallback(_handle, [](GLFWwindow*, double xoffset, double yoffset) {
        event_handler ::fire_event(string_id("MouseScroll"),
                                   new MouseScroll(xoffset, yoffset));
    });
    glfwSetWindowCloseCallback(_handle, [](GLFWwindow*) {
        event_handler ::fire_event(string_id("WindowClose"),
                                   new WindowClose);
    });
    glfwSetWindowSizeCallback(_handle, [](GLFWwindow*, int width, int height) {
        event_handler ::fire_event(string_id("WindowSize"),
                                   new WindowSize(width, height));
    });
    glfwSetFramebufferSizeCallback(_handle, [](GLFWwindow*, int width, int height) {
        event_handler ::fire_event(string_id("FramebufferSize"),
                                   new FramebufferSize(width, height));
    });
    glfwSetWindowContentScaleCallback(_handle, [](GLFWwindow*, float xscale, float yscale) {
        event_handler ::fire_event(string_id("WindowContentScale"),
                                   new WindowContentScale(xscale, yscale));
    });
    glfwSetWindowPosCallback(_handle, [](GLFWwindow*, int xpos, int ypos) {
        event_handler ::fire_event(string_id("WindowPos"),
                                   new WindowPos(xpos, ypos));
    });
    glfwSetWindowIconifyCallback(_handle, [](GLFWwindow*, int iconified) {
        event_handler ::fire_event(string_id("WindowIconify"),
                                   new WindowIconify(iconified));
    });
    glfwSetWindowMaximizeCallback(_handle, [](GLFWwindow*, int maximized) {
        event_handler ::fire_event(string_id("WindowMaximize"),
                                   new WindowMaximize(maximized));
    });
    glfwSetWindowFocusCallback(_handle, [](GLFWwindow*, int focused) {
        event_handler ::fire_event(string_id("WindowFocused"),
                                   new WindowFocused(focused));
    });
    glfwSetWindowRefreshCallback(_handle, [](GLFWwindow*) {
        event_handler ::fire_event(string_id("WindowRefresh"),
                                   new WindowRefresh);
    });
}
}