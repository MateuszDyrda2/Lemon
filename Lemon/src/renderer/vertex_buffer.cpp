#include <lemon/renderer/vertex_buffer.h>

namespace lemon {
vertex_buffer::vertex_buffer(size_type byteSize)
{
    glGenBuffers(1, &handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, byteSize, nullptr, GL_STREAM_DRAW);
}
vertex_buffer::~vertex_buffer()
{
    glDeleteBuffers(1, &handle);
}
void vertex_buffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, handle);
}
void vertex_buffer::enable_vertex_attrib(size_type index, size_type elementCount, GLenum type,
                                         bool normalized, size_type stride, size_type startsAt)
{
    glVertexAttribPointer(index, elementCount, type, normalized, stride, (void*)startsAt);
    glEnableVertexAttribArray(index);
}
} // namespace lemon
