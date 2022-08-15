#include <platform/window.h>

#include <core/logger.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace lemon {
window::window(const std::string& name, const ivec2& size):
    name(name), size(size)
{
    if(!glfwInit()) logger::fatal("GLFW initialization failed");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = reinterpret_cast<void*>(glfwCreateWindow(size.x, size.y, name.c_str(), NULL, NULL));
    if(!handle) logger::fatal("Window or OpenGL context creation failed");

    glfwSetErrorCallback(+[](int err, const char* desc) {
        logger::error("GLFW error: {} -> {}", err, desc);
    });
    glfwMakeContextCurrent(reinterpret_cast<GLFWwindow*>(handle));
    glfwSwapInterval(1); // fps locked to monitors
    logger::info("Window {} created <{}x{}>", name, size.x, size.y);
}
window::~window()
{
    logger::info("Window {} destroyed", name);
}
bool window::update()
{
    glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(handle));
    glfwPollEvents();
    return !glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(handle));
}
} // namespace lemon
