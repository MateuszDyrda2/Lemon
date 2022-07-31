#include <lemon/rendering/rendering_context.h>

#include <lemon/core/logger.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace lemon {
rendering_context::rendering_context()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logger::fatal("Failed to initialize GLAD");
    }
    logger::info("Rendering context created");
}
rendering_context::~rendering_context()
{
    logger::info("Rendering context destroyed");
}
void rendering_context::enable_blending()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void rendering_context::set_viewport(const ivec4& vp)
{
    glViewport(vp.x, vp.y, vp.z, vp.w);
}
void rendering_context::clear_screen(const color& c)
{
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT);
}
void rendering_context::draw_arrays(GLenum mode, std::size_t first, std::size_t count)
{
    glDrawArrays(mode, first, GLsizei(count));
}
void rendering_context::draw_elements(GLenum mode, std::size_t count, GLenum type, void* indices)
{
    glDrawElements(mode, count, type, indices);
}
void rendering_context::draw_arrays_instanced(GLenum mode, std::size_t first, std::size_t count,
                                              std::size_t instanceCount)
{
    glDrawArraysInstanced(mode, first, count, instanceCount);
}
void rendering_context::draw_elements_instanced(GLenum mode, std::size_t count, GLenum type,
                                                const void* indices, std::size_t instanceCount)
{
    glDrawElementsInstanced(mode, count, type, indices, instanceCount);
}
} // namespace lemon
