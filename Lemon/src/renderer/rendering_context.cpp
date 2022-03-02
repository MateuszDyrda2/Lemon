#include <lemon/renderer/rendering_context.h>

#include <lemon/core/logger.h>
#include <lemon/window/window.h>

namespace lemon {
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
void rendering_context::clear_screen(const glm::vec4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
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
ptr<rendering_context::texture_object> rendering_context::generate_texture()
{
    texture_object* obj = new texture_object;
    glGenTextures(1, &obj->texture_id);
    return obj;
}
ptr<rendering_context::texture_object>
rendering_context::texture_object::set_parameter(int param, int value)
{
    glTexParameteri(GL_TEXTURE_2D, param, value);
    return this;
}
ptr<rendering_context::texture_object>
rendering_context::texture_object::create_image(
    int level, int internalFormat, ivec2 size, GLenum format, GLenum type, const void* data)
{
    glTexImage2D(GL_TEXTURE_2D, level, internalFormat, size.x, size.y, 0, format, type, data);
    return this;
}
ptr<rendering_context::texture_object>
rendering_context::texture_object::set_unpack_alignment(int value)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, value);
    return this;
}
ptr<rendering_context::texture_object>
rendering_context::texture_object::bind()
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
    return this;
}
ptr<rendering_context::texture_object>
rendering_context::texture_object::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    return this;
}
void rendering_context::destroy_texture(texture_object** handle)
{
    glDeleteTextures(1, &((*handle)->texture_id));
    delete(*handle);
    *handle = nullptr;
}
} // namespace lemon
