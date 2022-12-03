#include <rendering/rendering_context.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <rendering/gl_errors.h>

namespace lemon {
rendering_context::rendering_context()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logger::fatal("Failed to initialize GLAD");
    }
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glDepthFunc(GL_LESS));
    GL_CHECK(glEnable(GL_BLEND));
    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    logger::info("Rendering context created");
}
rendering_context::~rendering_context()
{
    logger::info("Rendering context destroyed");
}
void rendering_context::enable_blending()
{
    GL_CHECK(glEnable(GL_BLEND));
    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}
void rendering_context::set_viewport(const ivec4& vp)
{
    GL_CHECK(glViewport(vp.x, vp.y, vp.z, vp.w));
}
void rendering_context::clear_screen(const color& c)
{
    GL_CHECK(glClearColor(c.r, c.g, c.b, c.a));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
void rendering_context::draw_arrays(GLenum mode, std::size_t first, std::size_t count)
{
    GL_CHECK(glDrawArrays(mode, first, static_cast<GLint>(count)));
}
void rendering_context::draw_elements(GLenum mode, std::size_t count, GLenum type, void* indices)
{
    GL_CHECK(glDrawElements(mode, static_cast<GLsizei>(count), type, indices));
}
void rendering_context::draw_arrays_instanced(GLenum mode, std::size_t first, std::size_t count,
                                              std::size_t instanceCount)
{
    GL_CHECK(glDrawArraysInstanced(mode, static_cast<GLint>(first), static_cast<GLsizei>(count), static_cast<GLsizei>(instanceCount)));
}
void rendering_context::draw_elements_instanced(GLenum mode, std::size_t count, GLenum type,
                                                const void* indices, std::size_t instanceCount)
{
    GL_CHECK(glDrawElementsInstanced(mode, static_cast<GLsizei>(count), type, indices, static_cast<GLsizei>(instanceCount)));
}
} // namespace lemon
