#pragma once

#include <core/lemon_types.h>
#include <core/math/vec2.h>

namespace lemon::detail {
/** Axis Aligned Bounding Box */
struct AABB
{
    vec2 min, max;
};

inline f32 AABB_area(AABB aabb)
{
    return (aabb.max.x - aabb.min.x) * (aabb.max.y - aabb.min.y);
}

inline vec2 min(const vec2& lhs, const vec2& rhs)
{
    return vec2(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y));
}

inline vec2 max(const vec2& lhs, const vec2& rhs)
{
    return vec2(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y));
}

inline AABB AABB_union(const AABB& a, const AABB& b)
{
    return AABB{
        .min = min(a.min, b.min),
        .max = max(a.max, b.max)
    };
}

inline bool intersects(const AABB& lhs, const AABB& rhs)
{
    return (lhs.max.x >= rhs.min.x && rhs.max.x >= lhs.min.x)
           && (lhs.max.y >= rhs.min.y && rhs.max.y >= lhs.min.y);
}
}
