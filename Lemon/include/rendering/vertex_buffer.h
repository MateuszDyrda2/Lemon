/** @file vertex_buffer.h
 * @brief File containing definition of a vertex buffer
 */
#pragma once

#include <core/defines.h>
#include <core/lemon_types.h>

#include <glad/glad.h>

namespace lemon {
/** Class representing a vertex buffer */
class LEMON_API vertex_buffer
{
  public:
    /** @brief Creates a vertex buffer
     * @param byteSize number of bytes the buffer should contain
     */
    vertex_buffer(std::size_t byteSize);
    /** @brief Creates a vertex buffer and assigns data to it
     * @param data data to assign to vertex buffer
     * @param byteSize number of bytes to assign
     */
    template<class T>
    vertex_buffer(T* data, std::size_t byteSize)
    {
        glGenBuffers(1, &handle);
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
    }
    ~vertex_buffer();
    /** @brief Binds the buffer
     * @return this
     */
    vertex_buffer* bind();
    /** @brief Changes data in the buffer
     * @param offset offset to change the data from
     * @param data new data
     * @return this
     */
    template<class T>
    vertex_buffer* add_subdata(std::size_t offset, std::size_t size, T* data)
    {
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        return this;
    }
    /** @return buffer handle */
    u32 get_handle() const { return handle; }
    /** @brief Enables vertex attribute
     * @param index attribute index
     * @param elementCount number of elements
     * @param type enum type
     * @param normlized is the attr normalized
     * @param stride stride
     * @param startsAt start offset
     * @return this
     */
    vertex_buffer* enable_vertex_attrib(std::size_t index, std::size_t elementCount, GLenum type,
                                        bool normalized, std::size_t stride, std::size_t startsAt);

  private:
    u32 handle;
};
} // namespace lemon
