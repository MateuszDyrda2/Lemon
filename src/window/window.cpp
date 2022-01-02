#include <river/window/window.h>

#include <river/core/logger.h>
#include <river/input/key_codes.h>

namespace river {
window::window(const std::string& name, size_type width, size_type height):
    _name(name), _width(width), _height(height), eventHandler(services::get<event_handler>())
{
    if(!glfwInit())
    {
        LOG_FATAL("GLFW initialization failure");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(!(_handle = glfwCreateWindow(width, height, name.c_str(), NULL, NULL)))
    {
        LOG_FATAL("Window or OpenGL context creation failed");
    }
    glfwMakeContextCurrent(_handle);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(_handle, (void*)eventHandler);
    setup_callbacks();
    windowResize = eventHandler->subscribe<int, int>(
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
bool window::should_close()
{
    return !!glfwWindowShouldClose(_handle);
}
void window::end_frame()
{
    glfwSwapBuffers(_handle);
    glfwPollEvents();
}
void window::setup_callbacks()
{

    glfwSetKeyCallback(_handle, [](GLFWwindow* w, int k, int scancode, int action, int mods) {
        auto handler = (event_handler*)glfwGetWindowUserPointer(w);
        handler->dispatch(string_id("KeyPressed"), key::keycode(k),
                          scancode, key::action(action), key::keymod(mods));
    });
    glfwSetWindowCloseCallback(_handle, [](GLFWwindow* w) {
        auto handler = (event_handler*)glfwGetWindowUserPointer(w);
        handler->dispatch(string_id("WindowClose"));
    });
    glfwSetWindowSizeCallback(_handle, [](GLFWwindow* w, int width, int height) {
        auto handler = (event_handler*)glfwGetWindowUserPointer(w);
        handler->dispatch(string_id("WindowSize"), width, height);
    });
    glfwSetFramebufferSizeCallback(_handle, [](GLFWwindow* w, int width, int height) {
        auto handler = (event_handler*)glfwGetWindowUserPointer(w);
        handler->dispatch(string_id("FramebufferSize"), width, height);
    });
    glfwSetWindowContentScaleCallback(_handle, [](GLFWwindow* w, float xscale, float yscale) {
        auto handler = (event_handler*)glfwGetWindowUserPointer(w);
        handler->dispatch(string_id("WindowContentScale"), xscale, yscale);
    });
    glfwSetWindowPosCallback(_handle, [](GLFWwindow* w, int xpos, int ypos) {
        auto handler = (event_handler*)glfwGetWindowUserPointer(w);
        handler->dispatch(string_id("WindowPos"), xpos, ypos);
    });
    glfwSetWindowIconifyCallback(_handle, [](GLFWwindow* w, int iconified) {
        auto handler = (event_handler*)glfwGetWindowUserPointer(w);
        handler->dispatch(string_id("WindowIconify"), iconified);
    });
    glfwSetWindowMaximizeCallback(_handle, [](GLFWwindow* w, int maximized) {
        auto handler = (event_handler*)glfwGetWindowUserPointer(w);
        handler->dispatch(string_id("WindowMaximize"), maximized);
    });
    glfwSetWindowFocusCallback(_handle, [](GLFWwindow* w, int focused) {
        auto handler = (event_handler*)glfwGetWindowUserPointer(w);
        handler->dispatch(string_id("WindowFocused"), focused);
    });
    glfwSetWindowRefreshCallback(_handle, [](GLFWwindow* w) {
        auto handler = (event_handler*)glfwGetWindowUserPointer(w);
        handler->dispatch(string_id("WindowRefresh"));
    });
}
}