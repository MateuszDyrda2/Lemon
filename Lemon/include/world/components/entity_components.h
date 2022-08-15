#pragma once

#include <core/defines.h>
#include <core/hash_string.h>

namespace lemon {
struct LEMON_API destroy_m
{ };

struct LEMON_API enable_m
{ };

struct LEMON_API disable_m
{ };

struct LEMON_API enabled_t
{ };

struct LEMON_API tag
{
    hash_str nameid;
};
}
