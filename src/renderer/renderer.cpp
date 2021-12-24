#include <river/renderer/renderer.h>

#include <river/core/assert.h>
#include <river/core/logger.h>

#include <river/window/window.h>

namespace river {
renderer::renderer(window* w):
    _window(w)
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // failed to initialize glad
        LOG_FATAL("Failed to initialize glad");
    }

    framebufferSize = event_handler::subscribe<size_type, size_type>("FramebufferSize", [](size_type width, size_type height) {
        LOG_MESSAGE("Framebuffer resize: %dx%d", width, height);
        glViewport(0, 0, width, height);
    });
    glViewport(0, 0, _window->get_width(), _window->get_height());
}
renderer::~renderer()
{
    framebufferSize.dispose();
}
void renderer::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
} // namespace river
