#include <lemon/platform/window.h>

#include <lemon/core/game.h>
#include <lemon/platform/key_codes.h>

#include <lemon/platform/window_events.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace lemon {
event window::onKeyPressed;
event window::onMouseButtonPressed;
event window::onMouseScroll;
event window::onWindowClose;
event window::onWindowSize;
event window::onFramebufferSize;
event window::onWindowContentScale;
event window::onWindowPos;
event window::onWindowIconify;
event window::onWindowMaximize;
event window::onWindowFocused;
event window::onWindowRefresh;

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
    glfwSwapInterval(1); // fps locked to 60

    glfwSetWindowUserPointer((GLFWwindow*)_handle, (void*)this);
    setup_callbacks((GLFWwindow*)_handle);
    window::onWindowSize.register_observer([this](event_args* a) {
        auto s = (WindowSize*)a;
        size   = ivec2(s->width, s->height);
        LOG_MESSAGE("Window resize: %dx%d", size.x, size.y);
    },
                                           string_id("window::onWindowSize"));

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
    glfwSetKeyCallback(_handle, [](GLFWwindow* w, int k, int scancode, int action, int mods) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onKeyPressed.notify(
                new KeyPressed(
                    key::keycode(k),
                    scancode,
                    key::action(action),
                    key::keymod(mods)));
    });
    glfwSetMouseButtonCallback(_handle, [](GLFWwindow* w, int button, int action, int mods) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onMouseButtonPressed.notify(
                new MouseButtonPressed(
                    key::mouse(button),
                    key::action(action),
                    key::keymod(mods)));
    });
    glfwSetScrollCallback(_handle, [](GLFWwindow* w, double xoffset, double yoffset) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onMouseScroll.notify(
                new MouseScroll(
                    xoffset,
                    yoffset));
    });
    glfwSetWindowCloseCallback(_handle, [](GLFWwindow* w) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onWindowClose.notify(
                new WindowClose);
    });
    glfwSetWindowSizeCallback(_handle, [](GLFWwindow* w, int width, int height) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onWindowSize.notify(
                new WindowSize(
                    width,
                    height));
    });
    glfwSetFramebufferSizeCallback(_handle, [](GLFWwindow* w, int width, int height) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onFramebufferSize.notify(
                new FramebufferSize(
                    width,
                    height));
    });
    glfwSetWindowContentScaleCallback(_handle, [](GLFWwindow* w, float xscale, float yscale) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onWindowContentScale.notify(
                new WindowContentScale(
                    xscale,
                    yscale));
    });
    glfwSetWindowPosCallback(_handle, [](GLFWwindow* w, int xpos, int ypos) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onWindowPos.notify(
                new WindowPos(
                    xpos,
                    ypos));
    });
    glfwSetWindowIconifyCallback(_handle, [](GLFWwindow* w, int iconified) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onWindowIconify.notify(
                new WindowIconify(
                    iconified));
    });
    glfwSetWindowMaximizeCallback(_handle, [](GLFWwindow* w, int maximized) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onWindowMaximize.notify(
                new WindowMaximize(
                    maximized));
    });
    glfwSetWindowFocusCallback(_handle, [](GLFWwindow* w, int focused) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onWindowFocused.notify(
                new WindowFocused(
                    focused));
    });
    glfwSetWindowRefreshCallback(_handle, [](GLFWwindow* w) {
        ((window*)(glfwGetWindowUserPointer(w)))
            ->onWindowRefresh.notify(
                new WindowRefresh);
    });
}
}