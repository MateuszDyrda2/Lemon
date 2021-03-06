#pragma once

#include "defines.h"
#include "string_id.h"

namespace lemon {
class LEMON_PUBLIC system
{
  public:
    system()                                            = default;
    virtual ~system()                                   = default;
    virtual const char* get_name() const noexcept       = 0;
    virtual constexpr string_id get_id() const noexcept = 0;
};
#define LEMON_REGISTER_SYSTEM(_name)                                          \
    virtual const char* get_name() const noexcept override { return #_name; } \
    virtual constexpr string_id get_id() const noexcept override { return string_id(#_name); }
} // namespace lemon