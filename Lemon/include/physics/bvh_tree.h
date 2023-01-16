/** @file bvh_tree.h
 * @brief BVH tree implementation
 */
#pragma once

#include "detail/physics_utils.h"

#include <core/defines.h>
#include <core/lemon_types.h>

#include <list>
#include <unordered_map>
#include <vector>

namespace lemon {
/** Bounding Volume Hierarchy tree implementation
 * https://en.wikipedia.org/wiki/Bounding_volume_hierarchy
 */
class LEMON_API bvh_tree
{
    using index_t                      = i32;
    using AABB                         = detail::AABB;
    static constexpr index_t nullIndex = -1;
    static constexpr f32 fattenBy      = 0.05f;
    struct node
    {
        AABB box;
        u32 entityId;
        index_t parentIndex;
        index_t childA;
        index_t childB;
        index_t nextFree;
        bool isLeaf;
        bool isActive;
        i32 height;
    };

  public:
    /** @brief Creates the tree */
    bvh_tree();
    ~bvh_tree();
    /** @brief Inserts a single bounding box for entity
     * @param entityId id of the inserted leaf
     * @param box Bounding box of the entity */
    void insert_leaf(u32 entityId, const AABB& box);
    /** @brief Removes entity from the tree
     * @param entityId id of the entity to remove */
    void remove_leaf(u32 entityId);
    /** @brief Update the bounding box of the entity
     * @param entityId id of the entity to update
     * @param box new bounding box */
    void update_leaf(u32 entityId, const AABB& box);
    /** @brief Get a list of colliding bounding boxes
     * @param entityId entity to check against
     * @return list of colliding entities */
    std::list<u32> query_tree(u32 entityId);
    /** @brief Get a list of entities colliding with a point
     * @param position position to intersect
     * @return list of colliding entities */
    std::list<u32> raycast(vec2 position);
    /** @return Get number of nodes */
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

    void fatten_box(AABB& box);
    void initialize_new_parent(index_t oldParentIndex, index_t newParentIndex, index_t siblingIndex, AABB& box);
    void create_parent(index_t oldParentIndex, index_t newParentIndex, index_t siblingIndex, index_t leafIndex);
    void create_parent_root(index_t oldParentIndex, index_t newParentIndex, index_t siblingIndex, index_t leafIndex);
    index_t rotate_right(index_t index, index_t childAIndex, index_t childBIndex);
    index_t rotate_left(index_t index, index_t childAIndex, index_t childBIndex);
    void rotate_right_left(index_t index, index_t childAIndex, index_t childBIndex, index_t childBchildAIndex, index_t childBchildBIndex);
    void rotate_right_right(index_t index, index_t childAIndex, index_t childBIndex, index_t childBchildAIndex, index_t childBchildBIndex);
    void rotate_left_left(index_t index, index_t childAIndex, index_t childBIndex, index_t childAchildAIndex, index_t childAchildBIndex);
    void rotate_left_right(index_t index, index_t childAIndex, index_t childBIndex, index_t childAchildAIndex, index_t childAchildBIndex);
};
}
