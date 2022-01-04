#include <lemon/renderer/vertex_array.h>

namespace lemon {
vertex_array::vertex_array()
{
    glGenVertexArrays(1, &handle);
}
vertex_array::~vertex_array()
{
    glDeleteVertexArrays(1, &handle);
}
void vertex_array::bind()
{
    glBindVertexArray(handle);
}
void vertex_array::unbind()
{
    glBindVertexArray(0);
}
void vertex_array::add_vertex_buffer(owned<vertex_buffer>&& vertex_buffer)
{
    vbo = std::move(vertex_buffer);
    glBindVertexArray(handle);
    vbo->bind();
}
} // namespace lemon
