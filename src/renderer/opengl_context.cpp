#include <river/renderer/opengl_context.h>

#include <river/core/logger.h>
#include <river/window/window.h>

namespace river {
opengl_context::opengl_context()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_FATAL("Failed to initialize GLAD");
    }
}
opengl_context::~opengl_context()
{
}
void opengl_context::set_viewport(const glm::ivec4& vp)
{
    glViewport(vp.x, vp.y, vp.z, vp.w);
}
void opengl_context::clear_screen(const glm::vec4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}
void opengl_context::draw_arrays(GLenum mode, size_type first, size_type count)
{
    glDrawArrays(mode, first, count);
}
void opengl_context::draw_elements(GLenum mode, size_type count, GLenum type, void* indices)
{
    glDrawElements(type, count, type, indices);
}
void opengl_context::draw_arrays_instanced(GLenum mode, size_type first, size_type count,
                                           size_type instanceCount)
{
    glDrawArraysInstanced(mode, first, count, instanceCount);
}
void opengl_context::draw_elements_instanced(GLenum mode, size_type count, GLenum type,
                                             const void* indices, size_type instanceCount)
{
    glDrawElementsInstanced(mode, count, type, indices, instanceCount);
}
} // namespace river
