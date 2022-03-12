#include <lemon/rendering/rendering_context.h>

#include <lemon/core/logger.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace lemon {
rendering_context::rendering_context()
{
    create();
}
rendering_context::~rendering_context()
{
    drop();
}
void rendering_context::create()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_FATAL("Failed to initialize GLAD");
    }
    LOG_MESSAGE("Rendering context created");
}
void rendering_context::drop()
{
    LOG_MESSAGE("Rendering context destroyed");
}
void rendering_context::enable_blending()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void rendering_context::set_viewport(const glm::ivec4& vp)
{
    glViewport(vp.x, vp.y, vp.z, vp.w);
}
void rendering_context::clear_screen(const color& c)
{
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT);
}
void rendering_context::draw_arrays(GLenum mode, size_type first, size_type count)
{
    glDrawArrays(mode, first, count);
}
void rendering_context::draw_elements(GLenum mode, size_type count, GLenum type, void* indices)
{
    glDrawElements(mode, count, type, indices);
}
void rendering_context::draw_arrays_instanced(GLenum mode, size_type first, size_type count,
                                              size_type instanceCount)
{
    glDrawArraysInstanced(mode, first, count, instanceCount);
}
void rendering_context::draw_elements_instanced(GLenum mode, size_type count, GLenum type,
                                                const void* indices, size_type instanceCount)
{
    glDrawElementsInstanced(mode, count, type, indices, instanceCount);
}
} // namespace lemon
