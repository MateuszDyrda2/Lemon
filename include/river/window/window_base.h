#pragma once

#include <river/core/basic_types.h>
#include <river/service/service.h>

namespace river {
class window_base : public service
{
  public:
    window_base() { }
    virtual ~window_base() { }
    virtual bool end_frame() { return false; }

    virtual size_type get_width() const { return 0; }
    virtual size_type get_height() const { return 0; }

    virtual void* get_handle() { return nullptr; }
};
} // namespace river