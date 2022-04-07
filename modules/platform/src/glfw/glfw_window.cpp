#include <lemon/platform/window.h>

#include <lemon/core/game.h>
#include <lemon/platform/key_codes.h>

#include <lemon/core/instrumentor.h>
#include <lemon/platform/window_events.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace lemon {
void setup_callbacks(GLFWwindow* _handle);
window::window(size_type width, size_type height, event_bus& ebus):
    ebus(ebus), _name(game::get_settings().gameName), size(width, height)
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
    glfwSwapInterval(0); // fps locked to 60

    glfwSetWindowUserPointer((GLFWwindow*)_handle, (void*)this);
    setup_callbacks(_handle);
    ebus.sink(string_id("WindowSize")) += this;
    glfwSetErrorCallback([](int /* error */, const char* description) {
        LOG_ERROR("GLFWError: %s", description);
    });
    LOG_MESSAGE("Window created %dx%d", width, height);
}
window::~window()
{
    ebus.sink(string_id("WindowSize")) -= this;
    glfwDestroyWindow((GLFWwindow*)_handle);
    glfwTerminate();
    LOG_MESSAGE("Window destroyed");
}
void window::on_event(event* e)
{
    auto ev = static_cast<WindowSize*>(e);
    size    = ivec2(ev->width, ev->height);
}
bool window::end_frame()
{
    LEMON_PROFILE_FUNCTION();
    glfwSwapBuffers((GLFWwindow*)_handle);
    glfwPollEvents();
    return !!glfwWindowShouldClose((GLFWwindow*)_handle);
}
void window::setup_callbacks(native_handle_t handle)
{
    auto _handle = (GLFWwindow*)handle;
    glfwSetKeyCallback(_handle, [](GLFWwindow* w, int k, int scancode, int action, int mods) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new KeyPressed(key::keycode(k), scancode, key::action(action), key::keymod(mods)));
    });
    glfwSetMouseButtonCallback(_handle, [](GLFWwindow* w, int button, int action, int mods) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new MouseButtonPressed(key::mouse(button), key::action(action), key::keymod(mods)));
    });
    glfwSetScrollCallback(_handle, [](GLFWwindow* w, double xoffset, double yoffset) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new MouseScroll(xoffset, yoffset));
    });
    glfwSetWindowCloseCallback(_handle, [](GLFWwindow* w) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new WindowClose);
    });
    glfwSetWindowSizeCallback(_handle, [](GLFWwindow* w, int width, int height) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new WindowSize(width, height));
    });
    glfwSetFramebufferSizeCallback(_handle, [](GLFWwindow* w, int width, int height) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new FramebufferSize(width, height));
    });
    glfwSetWindowContentScaleCallback(_handle, [](GLFWwindow* w, float xscale, float yscale) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new WindowContentScale(xscale, yscale));
    });
    glfwSetWindowPosCallback(_handle, [](GLFWwindow* w, int xpos, int ypos) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new WindowPos(xpos, ypos));
    });
    glfwSetWindowIconifyCallback(_handle, [](GLFWwindow* w, int iconified) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new WindowIconify(iconified));
    });
    glfwSetWindowMaximizeCallback(_handle, [](GLFWwindow* w, int maximized) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new WindowMaximize(maximized));
    });
    glfwSetWindowFocusCallback(_handle, [](GLFWwindow* w, int focused) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new WindowFocused(focused));
    });
    glfwSetWindowRefreshCallback(_handle, [](GLFWwindow* w) {
        ((window*)glfwGetWindowUserPointer(w))
            ->ebus.fire(
                new WindowRefresh);
    });
}
}