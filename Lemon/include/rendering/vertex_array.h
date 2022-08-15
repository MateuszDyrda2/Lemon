#pragma once

#include "vertex_buffer.h"
#include <memory>

namespace lemon {
class LEMON_API vertex_array
{
  public:
    vertex_array();
    ~vertex_array();

    void bind();
    void unbind();

    vertex_buffer* add_vertex_buffer(std::unique_ptr<vertex_buffer>&& vertex_buffer);

    u32 get_handle() const { return handle; }
    vertex_buffer* get_vbo() { return vbo.get(); }

  private:
    u32 handle;
    std::unique_ptr<vertex_buffer> vbo;
};
} // namespace lemon
