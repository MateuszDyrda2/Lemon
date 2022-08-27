#pragma once

#include "../reflection.h"

#include <core/defines.h>
#include <core/hash_string.h>

namespace lemon {
struct LEMON_API destroy_m
{ };

struct LEMON_API enable_m
{ };

struct LEMON_API disable_m
{ };

struct LEMON_API [[lemon::tag]] enabled_t
{ };

struct LEMON_API [[lemon::component]] tag
{
    [[lemon::field]] hash_str id;
};
}
