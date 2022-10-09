#include <physics/bvh_tree.h>

namespace lemon {
f32 AABB_area(AABB aabb)
{
    const auto d = aabb.max - aabb.min;
    return d.x * d.y;
}

vec2 min(const vec2& lhs, const vec2& rhs)
{
    return vec2(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y));
}

vec2 max(const vec2& lhs, const vec2& rhs)
{
    return vec2(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y));
}

AABB AABB_union(const AABB& a, const AABB& b)
{
    return AABB{
        .min = min(a.min, b.min),
        .max = max(a.max, b.max)
    };
}

bool intersects(const AABB& lhs, const AABB& rhs)
{
    return (lhs.max.x >= rhs.min.x && rhs.max.x >= lhs.min.x)
           && (lhs.max.y >= rhs.min.y && rhs.max.y >= lhs.min.y);
}

bvh_tree::bvh_tree()
{
}

bvh_tree::~bvh_tree()
{
}

void bvh_tree::insert_leaf(u32 entityId, const AABB& box)
{
}

void bvh_tree::remove_leaf(u32 entityId)
{
}

void bvh_tree::update_leaf(u32 entityId, const AABB& box)
{
}

std::list<u32> bvh_tree::query_tree(u32 entityId)
{
}

void bvh_tree::insert_node(index_t node)
{
}

void bvh_tree::remove_node(index_t node)
{
}

bvh_tree::index_t bvh_tree::rotate(index_t node)
{
}

f32 bvh_tree::compute_cost()
{
}

bvh_tree::index_t bvh_tree::find_sibling(const AABB& box)
{
}

bvh_tree::index_t bvh_tree::allocate_node()
{
}

void bvh_tree::deallocate_node(index_t node)
{
}
}
