/** @file vertex_array.h
 * @brief File containing definition of a vertex array
 */
#pragma once

#include "vertex_buffer.h"
#include <memory>

namespace lemon {
/** Class representing vertex array */
class LEMON_API vertex_array
{
  public:
    /** @brief Constructs a vertex array */
    vertex_array();
    ~vertex_array();
    /** @brief Binds the array */
    void bind();
    /** @brief Unbinds the array */
    void unbind();
    /** @brief Assigns a vertex buffer to the array
     * @param vertex_buffer buffer to assign
     * @return this
     */
    vertex_buffer* add_vertex_buffer(std::unique_ptr<vertex_buffer>&& vertex_buffer);
    /** @return handle */
    u32 get_handle() const { return handle; }
    /** @return vertex buffer */
    vertex_buffer* get_vbo() { return vbo.get(); }

  private:
    u32 handle;
    std::unique_ptr<vertex_buffer> vbo;
};
} // namespace lemon
