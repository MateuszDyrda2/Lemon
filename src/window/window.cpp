#include <river/window/window.h>

#include <river/core/logger.h>

namespace river {
window::window(const std::string& name, size_type width, size_type height):
    _name(name), _width(width), _height(height)
{
    if(!glfwInit())
    {
        LOG_FATAL("GLFW initialization failure");
        // initialization failure
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(!(_handle = glfwCreateWindow(width, height, name.c_str(), NULL, NULL)))
    {
        LOG_FATAL("Window or OpenGL context creation failed");
        // window or OpenGL context creation failed
    }
    glfwMakeContextCurrent(_handle);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(_handle, (void*)this);
    setup_callbacks();
    windowResize = event_handler::subscribe<size_type, size_type>(
        "WindowSize",
        [this](size_type width, size_type height) {
            LOG_MESSAGE("Window resize: %dx%d", width, height);
            _width  = width;
            _height = height;
        });

    glfwSetErrorCallback([](int /* error */, const char* description) {
        LOG_ERROR("GLFWError: %s", description);
    });
    LOG_MESSAGE("Window created %dx%d", width, height);
    glfwMakeContextCurrent(_handle);
}
window::~window()
{
    glfwDestroyWindow(_handle);
    glfwTerminate();
    LOG_MESSAGE("Window destroyed");
    windowResize.dispose();
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
    event_handler::register_event<int, int, int, int>("KeyPressed");
    event_handler::register_event("WindowClose");
    event_handler::register_event<int, int>("WindowSize");
    event_handler::register_event<int, int>("FramebufferSize");
    event_handler::register_event<float, float>("WindowContentScale");
    event_handler::register_event<int, int>("WindowPos");
    event_handler::register_event<int>("WindowIconify");
    event_handler::register_event<int>("WindowMaximize");
    event_handler::register_event<int>("WindowFocused");
    event_handler::register_event("WindowRefresh");

    glfwSetKeyCallback(_handle, [](GLFWwindow*, int key, int scancode, int action, int mods) {
        event_handler::dispatch("KeyPressed", key, scancode, action, mods);
    });
    glfwSetWindowCloseCallback(_handle, [](GLFWwindow*) {
        event_handler::dispatch("WindowClose");
    });
    glfwSetWindowSizeCallback(_handle, [](GLFWwindow*, int width, int height) {
        event_handler::dispatch("WindowSize", width, height);
    });
    glfwSetFramebufferSizeCallback(_handle, [](GLFWwindow*, int width, int height) {
        event_handler::dispatch("FramebufferSize", width, height);
    });
    glfwSetWindowContentScaleCallback(_handle, [](GLFWwindow*, float xscale, float yscale) {
        event_handler::dispatch("WindowContentScale", xscale, yscale);
    });
    glfwSetWindowPosCallback(_handle, [](GLFWwindow*, int xpos, int ypos) {
        event_handler::dispatch("WindowPos", xpos, ypos);
    });
    glfwSetWindowIconifyCallback(_handle, [](GLFWwindow*, int iconified) {
        event_handler::dispatch("WindowIconify", iconified);
    });
    glfwSetWindowMaximizeCallback(_handle, [](GLFWwindow*, int maximized) {
        event_handler::dispatch("WindowMaximize", maximized);
    });
    glfwSetWindowFocusCallback(_handle, [](GLFWwindow*, int focused) {
        event_handler::dispatch("WindowFocused", focused);
    });
    glfwSetWindowRefreshCallback(_handle, [](GLFWwindow*) {
        event_handler::dispatch("WindowRefresh");
    });
}
}