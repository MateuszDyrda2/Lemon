#include <river/window/window.h>

#include <river/core/logger.h>

namespace river {
window::window(std::string const& name, int width, int height):
    _name(name), _width(width), _height(height)
{
    if(!glfwInit())
    {
        // initialization failure
    }
    if(!(handle = glfwCreateWindow(width, height, name.c_str(), NULL, NULL)))
    {
        // window or OpenGL context creation failed
    }
    glfwMakeContextCurrent(handle);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(handle, (void*)this);
    glfwSetWindowSizeCallback(handle, [](GLFWwindow* handle, int width, int height) {
        auto wnd     = (window*)glfwGetWindowUserPointer(handle);
        wnd->_width  = width;
        wnd->_height = height;
    });
    LOG_MESSAGE("Window created %dx%d", width, height);
}
window::~window()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
    LOG_MESSAGE("Window destroyed");
}
bool window::should_close()
{
    return !!glfwWindowShouldClose(handle);
}
void window::end_frame()
{
    glfwSwapBuffers(handle);
    glfwPollEvents();
}
}