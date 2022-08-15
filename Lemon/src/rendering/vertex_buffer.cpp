#include <rendering/vertex_buffer.h>

namespace lemon {
vertex_buffer::vertex_buffer(std::size_t byteSize)
{
    glGenBuffers(1, &handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, byteSize, nullptr, GL_STREAM_DRAW);
}
vertex_buffer::~vertex_buffer()
{
    glDeleteBuffers(1, &handle);
}
vertex_buffer* vertex_buffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    return this;
}
vertex_buffer* vertex_buffer::enable_vertex_attrib(std::size_t index, std::size_t elementCount, GLenum type,
                                                   bool normalized, std::size_t stride, std::size_t startsAt)
{
    glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(elementCount), type, normalized, static_cast<GLsizei>(stride), (void*)startsAt);
    glEnableVertexAttribArray(static_cast<GLuint>(index));
    return this;
}
} // namespace lemon
