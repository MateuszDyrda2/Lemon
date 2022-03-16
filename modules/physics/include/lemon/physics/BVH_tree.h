#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

#include <list>
#include <queue>
#include <unordered_map>
#include <vector>

namespace lemon {
struct AABB
{
    vec2 min, max;
};

class BVH_tree
{
    using index_t                      = i32;
    static constexpr index_t nullIndex = -1;
    struct node
    {
        AABB box;
        u32 entityId;
        index_t parentIndex;
        index_t child1;
        index_t child2;
        index_t nextFree;
        bool isLeaf;
        i32 height;
    };

  public:
    BVH_tree();
    ~BVH_tree();

    void insert_leaf(u32 entityId, const AABB& box);
    void remove_leaf(u32 entityId);
    void update_leaf(u32 entityId, const AABB& box);
    std::list<u32> query_tree(const AABB& box);

  private:
    std::vector<node> nodes;
    std::unordered_map<u32, index_t> entityNodeMap;
    size_type nodeCount;
    index_t rootIndex;
    index_t nextFree;

  private:
    index_t rotate(index_t index);
    f32 compute_cost();
    index_t find_sibling(const AABB& box);
    index_t allocate_node();
    void deallocate_node(index_t index);
};
} // namespace lemon
