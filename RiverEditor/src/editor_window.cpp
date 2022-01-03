#include <RiverEditor/editor_window.h>

editor_window::editor_window(std::size_t width, std::size_t height):
    window_base(width, height)
{
    if(!glfwInit())
    {
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(!(_handle = glfwCreateWindow(width, height, "RiverEditor", NULL, NULL)))
    {
    }
    glfwMakeContextCurrent(_handle);
    glfwSwapInterval(1);
}
editor_window::~editor_window()
{
    glfwDestroyWindow(_handle);
    glfwTerminate();
}
bool editor_window::end_frame()
{
    glfwSwapBuffers(_handle);
    glfwPollEvents();
    return !!glfwWindowShouldClose(_handle);
}