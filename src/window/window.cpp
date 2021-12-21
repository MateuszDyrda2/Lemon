#include <river/window/window.h>

namespace river {
window::window(std::string const& name, int width, int height)
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
        auto wnd    = (window*)glfwGetWindowUserPointer(handle);
        wnd->width  = width;
        wnd->height = height;
    });
}
window::~window()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
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