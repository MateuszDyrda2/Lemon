#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/service/service.h>

#include <glm/vec2.hpp>

namespace lemon {
class window_base : public service
{
  public:
    window_base(size_type width, size_type height):
        _width(width), _height(height) { }
    virtual ~window_base() { }
    virtual bool end_frame() { return false; }

    size_type get_width() const { return _width; }
    size_type get_height() const { return _height; }
    glm::ivec2 get_size() const { return glm::ivec2{ _width, _height }; }

    virtual void* get_handle() { return nullptr; }

  protected:
    size_type _width, _height;
};
} // namespace lemon