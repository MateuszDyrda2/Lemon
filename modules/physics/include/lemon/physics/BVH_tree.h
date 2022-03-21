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
    static constexpr f32 fattenBy      = 0.05f;
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
    /** @brief Creates a BVH tree */
    BVH_tree();
    ~BVH_tree();

    /** @brief Inserts a newly created entity with AABB
     * @param entityId id of the entity
     * @param box Axis Aligned Bounding Box
     */
    void insert_leaf(u32 entityId, const AABB& box);
    /** @brief Removes previously inserted entity from the tree
     * @param entityId id of the entity to be removed
     */
    void remove_leaf(u32 entityId);
    /** @brief Updates the bounding box of the entity
     */
    void update_leaf(u32 entityId, const AABB& box);
    /** @brief Check for a collision of entity in the tree
     * (Broad phase)
     * @param entityId id of the entity containing the AABB to be checked against
     * @return a list of all the entities that overlap with the queried entity
     */
    std::list<u32> query_tree(u32 entityId);

  private:
    std::vector<node> nodes;
    std::unordered_map<u32, index_t> entityNodeMap;
    size_type nodeCount;
    index_t rootIndex;
    index_t nextFree;

  private:
    void insert_node(index_t node);
    void remove_node(index_t node);

    index_t rotate(index_t index);
    f32 compute_cost();
    index_t find_sibling(const AABB& box);
    index_t allocate_node();
    void deallocate_node(index_t index);
};
} // namespace lemon
