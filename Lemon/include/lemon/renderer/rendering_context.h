#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/service/service.h>

#include <glad/glad.h>
#include <glm/vec4.hpp>

namespace lemon {
class rendering_context : public service
{
  public:
    rendering_context();
    ~rendering_context();
    void enable_blending();
    void set_viewport(const glm::ivec4& vp);
    void clear_screen(const glm::vec4& color);
    void draw_arrays(GLenum mode, size_type first, size_type count);
    void draw_elements(GLenum mode, size_type count, GLenum type, void* indices);
    void draw_arrays_instanced(GLenum mode, size_type first, size_type count,
                               size_type instanceCount);
    void draw_elements_instanced(GLenum mode, size_type count, GLenum type,
                                 const void* indices, size_type instanceCount);
};
} // namespace lemon
