#include <river/window/window.h>

#include <river/core/logger.h>

namespace river {
window::window(const std::string& name, int width, int height):
    _name(name), _width(width), _height(height)
{
    if(!glfwInit())
    {
        LOG_FATAL("GLFW initialization failure");
        // initialization failure
    }
    if(!(_handle = glfwCreateWindow(width, height, name.c_str(), NULL, NULL)))
    {
        LOG_FATAL("Window or OpenGL context creation failed");
        // window or OpenGL context creation failed
    }
    glfwMakeContextCurrent(_handle);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(_handle, (void*)this);
    glfwSetWindowSizeCallback(_handle, [](GLFWwindow* handle, int w, int h) {
        auto wnd     = (window*)glfwGetWindowUserPointer(handle);
        wnd->_width  = w;
        wnd->_height = h;
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
}