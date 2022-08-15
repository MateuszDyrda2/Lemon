#pragma once

#include <core/defines.h>
#include <core/lemon_types.h>

#include <glad/glad.h>

namespace lemon {
class LEMON_API vertex_buffer
{
  public:
    vertex_buffer(std::size_t byteSize);
    template<class T>
    vertex_buffer(T* data, std::size_t byteSize)
    {
        glGenBuffers(1, &handle);
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
    }
    ~vertex_buffer();

    vertex_buffer* bind();
    template<class T>
    vertex_buffer* add_subdata(std::size_t offset, std::size_t size, T* data)
    {
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        return this;
    }

    u32 get_handle() const { return handle; }
    vertex_buffer* enable_vertex_attrib(std::size_t index, std::size_t elementCount, GLenum type,
                                        bool normalized, std::size_t stride, std::size_t startsAt);

  private:
    u32 handle;
};
} // namespace lemon
