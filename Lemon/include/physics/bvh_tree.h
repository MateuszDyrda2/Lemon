#pragma once

#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/vec2.h>

#include <list>
#include <unordered_map>
#include <vector>

namespace lemon {
/** Axis Aligned Bounding Box */
struct AABB
{
    vec2 min, max;
};
/** Bounding Volume Hierarchy tree implementation
 * https://en.wikipedia.org/wiki/Bounding_volume_hierarchy
 */
class LEMON_API bvh_tree
{
    using index_t                      = i32;
    static constexpr index_t nullIndex = -1;
    static constexpr f32 fattenBy      = 0.05f;
    struct node
    {
        AABB box;
        u32 entityId;
        index_t parentIndex;
        index_t childA;
        index_t childB;
        bool isLeaf;
        i32 height;
    };

  public:
    bvh_tree();
    ~bvh_tree();
    void insert_leaf(u32 entityId, const AABB& box);
    void remove_leaf(u32 entityId);
    void update_leaf(u32 entityId, const AABB& box);
    std::list<u32> query_tree(u32 entityId);
    std::size_t get_node_count() const { return nodeCount; }

  private:
    std::vector<node> nodes;
    std::unordered_map<u32, index_t> entityNodeMap;
    std::size_t nodeCount;
    index_t rootIndex;
    index_t nextFree;

  private:
    void insert_node(index_t node);
    void remove_node(index_t node);
    index_t rotate(index_t node);
    f32 compute_cost();
    index_t find_sibling(const AABB& box);
    index_t allocate_node();
    void deallocate_node(index_t node);
};
}
