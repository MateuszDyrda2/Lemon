#pragma once

#include "vertex_buffer.h"

namespace lemon {
class LEMON_PUBLIC vertex_array
{
  public:
    vertex_array();
    ~vertex_array();

    void bind();
    void unbind();

    ptr<vertex_buffer> add_vertex_buffer(owned<vertex_buffer>&& vertex_buffer);

    u32 get_handle() const { return handle; }
    ptr<vertex_buffer> get_vbo() { return vbo.get(); }

  private:
    u32 handle;
    owned<vertex_buffer> vbo;
};
} // namespace lemon
