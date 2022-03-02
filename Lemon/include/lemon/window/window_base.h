#pragma once

#include <lemon/core/basic_types.h>

#include <lemon/math/vec2.h>

namespace lemon {
class LEMON_PUBLIC window_base
{
  public:
    window_base(size_type width, size_type height);
    virtual ~window_base() = default;
    virtual bool end_frame() { return false; }

    inline size_type get_width() const { return _width; }
    inline size_type get_height() const { return _height; }
    inline ivec2 get_size() const { return ivec2{ _width, _height }; }

    virtual void* get_handle() { return nullptr; }

  protected:
    size_type _width, _height;
};
} // namespace lemon
