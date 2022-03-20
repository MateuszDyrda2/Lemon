#include <lemon/physics/BVH_tree.h>

#include <lemon/core/assert.h>

#include <algorithm>
#include <stack>

namespace lemon {
float AABB_area(AABB a)
{
    vec2 d = a.max - a.min;
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
    return AABB(min(a.min, b.min), max(a.max, b.max));
}
bool intersects(const AABB& lhs, const AABB& rhs)
{
    return (lhs.max.x >= rhs.min.x && rhs.max.x >= lhs.min.x)
           && (lhs.max.y >= rhs.min.y && rhs.max.y >= lhs.min.y);
}

BVH_tree::BVH_tree():
    nodes(10), entityNodeMap(), nodeCount(0), rootIndex(nullIndex), nextFree(nullIndex)
{
    nextFree = 0;
    for(index_t i = 0; i < nodes.size() - 1; ++i)
    {
        nodes[i].nextFree = i + 1;
    }
    nodes.back().nextFree = nullIndex;
}
BVH_tree::~BVH_tree()
{
}
void BVH_tree::insert_leaf(u32 entityId, const AABB& box)
{
    int leafIndex             = allocate_node();
    entityNodeMap[entityId]   = leafIndex;
    nodes[leafIndex].box      = box;
    nodes[leafIndex].entityId = entityId;
    nodes[leafIndex].isLeaf   = true;
    insert_node(leafIndex);
}
void BVH_tree::remove_leaf(u32 entityId)
{
    LEMON_ASSERT(entityNodeMap.contains(entityId));
    index_t index = entityNodeMap[entityId];
    remove_leaf(index);
    deallocate_node(index);
    entityNodeMap.erase(entityId);
}
void BVH_tree::update_leaf(u32 entityId, const AABB& box)
{
    index_t index = entityNodeMap[entityId];

    remove_node(index);

    nodes[index].box = box;

    insert_node(index);
}
void BVH_tree::insert_node(index_t leafIndex)
{
    auto& newNode   = nodes[leafIndex];
    const auto& box = newNode.box;

    if(nodeCount == 1)
    {
        rootIndex = leafIndex;
        return;
    }

    // stage 1: find the best sibling for the new leaf
    index_t sibling = find_sibling(box);

    // stage 2: create a new parent
    index_t oldParent         = nodes[sibling].parentIndex;
    index_t newParent         = allocate_node();
    auto& newParentNode       = nodes[newParent];
    newParentNode.box         = AABB_union(box, nodes[sibling].box);
    newParentNode.parentIndex = oldParent;
    newParentNode.isLeaf      = false;
    // AABB(box U siblingBox), entityId(0), parentId(oldParent), child1,2(0), isLeaf(false)
    // nodes.emplace_back(AABB_union(box, nodes[sibling].box), 0, oldParent, -1, -1, false);
    if(oldParent != nullIndex)
    {
        if(nodes[oldParent].child1 == sibling)
        {
            nodes[oldParent].child1 = newParent;
        }
        else
        {
            nodes[oldParent].child2 = newParent;
        }
        nodes[newParent].child1      = sibling;
        nodes[newParent].child2      = leafIndex;
        nodes[sibling].parentIndex   = newParent;
        nodes[leafIndex].parentIndex = newParent;
    }
    else
    {
        // the sibling was the root
        nodes[newParent].child1      = sibling;
        nodes[newParent].child2      = leafIndex;
        nodes[sibling].parentIndex   = newParent;
        nodes[leafIndex].parentIndex = newParent;
        rootIndex                    = newParent;
    }

    // stage 3: walk back up the tree refitting AABBs
    index_t index = nodes[leafIndex].parentIndex;
    while(index != nullIndex)
    {
        index = rotate(index);

        node& thisNode = nodes[index];
        node& child1   = nodes[thisNode.child1];
        node& child2   = nodes[thisNode.child2];

        thisNode.box    = AABB_union(child1.box, child2.box);
        thisNode.height = 1 + std::max(child1.height, child2.height);

        index = thisNode.parentIndex;
    }
}
void BVH_tree::remove_node(index_t index)
{
    if(index == rootIndex)
        rootIndex = nullIndex;
    else
    {
        node& currentNode        = nodes[index];
        index_t parentIndex      = currentNode.parentIndex;
        node& parentNode         = nodes[parentIndex];
        index_t siblingIndex     = parentNode.child1 == index ? parentNode.child2 : parentNode.child1;
        node& siblingNode        = nodes[siblingIndex];
        index_t grandparentIndex = parentNode.parentIndex;
        if(grandparentIndex != nullIndex)
        {
            node& grandparentNode = nodes[grandparentIndex];
            if(grandparentNode.child1 == parentIndex)
            {
                grandparentNode.child1 = siblingIndex;
            }
            else
            {
                grandparentNode.child2 = siblingIndex;
            }
            siblingNode.parentIndex = grandparentIndex;
            deallocate_node(parentIndex);

            index_t thisNodeIndex = grandparentIndex;
            while(thisNodeIndex != nullIndex)
            {
                thisNodeIndex = rotate(thisNodeIndex);

                node& thisNode = nodes[thisNodeIndex];
                node& child1   = nodes[thisNode.child1];
                node& child2   = nodes[thisNode.child2];

                thisNode.box    = AABB_union(child1.box, child2.box);
                thisNode.height = 1 + std::max(child1.height, child2.height);

                thisNodeIndex = thisNode.parentIndex;
            }
        }
        else
        {
            rootIndex               = siblingIndex;
            siblingNode.parentIndex = nullIndex;
            deallocate_node(parentIndex);
        }
        currentNode.parentIndex = nullIndex;
    }
}
f32 BVH_tree::compute_cost()
{
    f32 cost = 0.0f;
    std::for_each(nodes.begin(), nodes.end(), [&](const auto& node) {
        if(node.isLeaf == false) cost += AABB_area(node.box);
    });
    return cost;
}

BVH_tree::index_t BVH_tree::find_sibling(const AABB& box)
{
    f32 bestCost        = AABB_area(AABB_union(nodes[rootIndex].box, box));
    index_t bestSibling = rootIndex;
    std::queue<index_t> bestCandidates;
    bestCandidates.push(rootIndex);

    f32 inheritedCost = 0;
    while(!bestCandidates.empty())
    {
        index_t current = bestCandidates.front();
        bestCandidates.pop();
        // Cost of the node SA(node)
        f32 nodeCost = AABB_area(nodes[current].box);
        // Cost of the inserted node SA(inserted)
        f32 boxCost = AABB_area(box);
        // direct cost SA(node U inserted)
        f32 directCost = AABB_area(AABB_union(nodes[current].box, box));
        // const of choosing SA(node U inserted) + SumOf(deltaSA(prev_nodes))
        f32 costOfChoosing = directCost + inheritedCost;
        // inherited cost SumOf(deltaSA(prev_nodes)) + deltaSA(node)
        inheritedCost += directCost - nodeCost;
        // if cost of choosing this node is better than the best cost
        // update the best cost
        if(costOfChoosing < bestCost)
        {
            bestCost    = costOfChoosing;
            bestSibling = current;
        }
        // is it worth to explore the sub-tree of the current node
        f32 lowerBound = boxCost + inheritedCost;
        if(lowerBound < bestCost && !nodes[current].isLeaf)
        {
            bestCandidates.push(nodes[current].child1);
            bestCandidates.push(nodes[current].child2);
        }
    }
    return bestSibling;
}
BVH_tree::index_t BVH_tree::rotate(index_t index)
{
    LEMON_ASSERT(index != nullIndex);
    if(nodes[index].isLeaf || nodes[index].height < 2) return index;

    index_t child1 = nodes[index].child1;
    index_t child2 = nodes[index].child2;

    i32 balance = nodes[child2].height - nodes[child1].height;
    // rotate right branch up
    if(balance > 1)
    {
        index_t child2child1 = nodes[child2].child1;
        index_t child2child2 = nodes[child2].child2;

        nodes[child2].child1      = index;
        nodes[child2].parentIndex = nodes[index].parentIndex;
        nodes[index].parentIndex  = child2;

        if(nodes[child2].parentIndex != nullIndex)
        {
            if(nodes[nodes[child2].parentIndex].child1 == index)
            {
                nodes[nodes[child2].parentIndex].child1 = child2;
            }
            else
            {
                nodes[nodes[child2].parentIndex].child2 = child2;
            }
        }
        else
        {
            rootIndex = child2;
        }
        if(nodes[child2child1].height > nodes[child2child2].height)
        {
            nodes[child2].child2            = child2child1;
            nodes[index].child2             = child2child2;
            nodes[child2child2].parentIndex = index;
            nodes[index].box                = AABB_union(nodes[child1].box, nodes[child2child2].box);
            nodes[child1].box               = AABB_union(nodes[index].box, nodes[child2child1].box);

            nodes[index].height  = 1 + std::max(nodes[child1].height, nodes[child2child2].height);
            nodes[child1].height = 1 + std::max(nodes[index].height, nodes[child2child1].height);
        }
        else
        {
            nodes[child2].child2            = child2child2;
            nodes[index].child2             = child2child1;
            nodes[child2child1].parentIndex = index;
            nodes[index].box                = AABB_union(nodes[child1].box, nodes[child2child1].box);
            nodes[child1].box               = AABB_union(nodes[index].box, nodes[child2child2].box);

            nodes[index].height  = 1 + std::max(nodes[child1].height, nodes[child2child1].height);
            nodes[child1].height = 1 + std::max(nodes[index].height, nodes[child2child2].height);
        }
        return child2;
    }
    else if(balance < -1)
    {
        index_t child1child1 = nodes[child1].child1;
        index_t child1child2 = nodes[child1].child2;

        nodes[child1].child1      = index;
        nodes[child1].parentIndex = nodes[index].parentIndex;
        nodes[index].parentIndex  = child1;
        if(nodes[child1].parentIndex != nullIndex)
        {
            if(nodes[nodes[child1].parentIndex].child1 == index)
            {
                nodes[nodes[child1].parentIndex].child1 = child1;
            }
            else
            {
                nodes[nodes[child1].parentIndex].child2 = child1;
            }
        }
        else
        {
            rootIndex = child1;
        }
        if(nodes[child1child1].height > nodes[child1child2].height)
        {
            nodes[child1].child2            = child1child1;
            nodes[index].child1             = child1child2;
            nodes[child1child2].parentIndex = index;
            nodes[index].box                = AABB_union(nodes[child2].box, nodes[child1child2].box);
            nodes[child2].box               = AABB_union(nodes[index].box, nodes[child1child1].box);

            nodes[index].height  = 1 + std::max(nodes[child2].height, nodes[child1child2].height);
            nodes[child1].height = 1 + std::max(nodes[index].height, nodes[child1child1].height);
        }
        else
        {
            nodes[child1].child2            = child1child2;
            nodes[index].child1             = child1child1;
            nodes[child1child1].parentIndex = index;
            nodes[index].box                = AABB_union(nodes[child2].box, nodes[child1child1].box);
            nodes[child2].box               = AABB_union(nodes[index].box, nodes[child1child2].box);

            nodes[index].height  = 1 + std::max(nodes[child2].height, nodes[child1child1].height);
            nodes[child1].height = 1 + std::max(nodes[index].height, nodes[child1child2].height);
        }
        return child1;
    }
    return index;
}
std::list<u32> BVH_tree::query_tree(u32 entityId)
{
    std::list<u32> entities;
    if(auto current = entityNodeMap.find(entityId); current != entityNodeMap.end())
    {
        index_t queriedIndex = current->second;
        auto& queriedNode    = nodes[queriedIndex];

        if(!nodes.empty())
        {
            std::stack<index_t> queriedNodes;
            queriedNodes.push(rootIndex);
            while(!queriedNodes.empty())
            {
                index_t current   = queriedNodes.top();
                auto& currentNode = nodes[current];
                queriedNodes.pop();
                if(intersects(currentNode.box, queriedNode.box))
                {
                    if(currentNode.isLeaf)
                    {
                        if(currentNode.entityId != entityId)
                        {
                            entities.push_back(currentNode.entityId);
                        }
                    }
                    else
                    {
                        queriedNodes.push(currentNode.child1);
                        queriedNodes.push(currentNode.child2);
                    }
                }
            }
        }
    }
    return entities;
}
BVH_tree::index_t BVH_tree::allocate_node()
{
    if(nextFree == nullIndex)
    {
        auto size = nodes.size();
        nodes.resize(size + 10);
        index_t nextfree = nullIndex;
        for(index_t i = size; i < nodes.size(); ++i)
        {
            nodes[i].nextFree = nextfree;
            nextfree          = i;
        }
        nextFree = nodes.size() - 1;
    }
    index_t newNodeIndex = nextFree;
    auto& newNode        = nodes[newNodeIndex];
    newNode.parentIndex  = nullIndex;
    newNode.child1       = nullIndex;
    newNode.child2       = nullIndex;
    nodeCount += 1;
    nextFree = newNode.nextFree;
    return newNodeIndex;
}
void BVH_tree::deallocate_node(index_t index)
{
    auto& toDeallocate    = nodes[index];
    toDeallocate.nextFree = nextFree;
    nextFree              = index;
    nodeCount -= 1;
}
} // namespace lemon
