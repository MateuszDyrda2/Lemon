#pragma once

#include <lemon/core/basic_types.h>

#include <glad/glad.h>

namespace lemon {
class LEMON_PUBLIC vertex_buffer
{
  public:
    vertex_buffer(size_type byteSize);
    template<class T>
    vertex_buffer(T* data, size_type byteSize)
    {
        glGenBuffers(1, &handle);
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
    }
    ~vertex_buffer();

    void bind();
    template<class T>
    void add_subdata(size_type offset, size_type size, T* data)
    {
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    u32 get_handle() const { return handle; }
    void enable_vertex_attrib(size_type index, size_type elementCount, GLenum type,
                              bool normalized, size_type stride, size_type startsAt);

  private:
    u32 handle;
};
} // namespace lemon
