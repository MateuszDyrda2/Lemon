#pragma once

#include <core/lemon_types.h>

#include <limits>

namespace lemon {
using componentid = u32;

struct componentdef
{
    componentid id;
    u32 stride;
};

struct entityid
{
    union
    {
        struct
        {
            u32 alive : 1, enabled : 1, version : 6, id : 24;
        };
        u32 value;
    };

    using type                 = u32;
    static constexpr type null = (1UL << 24) - 1UL;

    constexpr entityid():
        id(null), alive(1), enabled(1), version(0) { }

    explicit constexpr entityid(type value):
        id(value), alive(1), enabled(1), version(0) { }

    explicit operator bool() const
    {
        return id != null;
    }

    constexpr bool operator==(const entityid& other) const
    {
        return id == other.id && version == other.version;
    }

    constexpr bool operator!=(const entityid& other) const
    {
        return !(*this == other);
    }
};
using entityid_t = entityid::type;
}
