#include <physics/bvh_tree.h>

#include <numeric>
#include <queue>
#include <stack>

namespace lemon {
// https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf

bvh_tree::bvh_tree():
    nodes(10), entityNodeMap(), nodeCount(0),
    rootIndex(nullIndex), nextFree(nullIndex)
{
    for(int i = nodes.size() - 1; i >= 0; --i)
    {
        nodes[i].nextFree = nextFree;
        nextFree          = i;
    }
}

bvh_tree::~bvh_tree()
{ }

void bvh_tree::insert_leaf(u32 entityId, const AABB& box)
{
    if(entityNodeMap.contains(entityId))
    {
        logger::error("The entity map already contains the entity with id: {}", entityId);
        return;
    }

    auto leafIndex          = allocate_node();
    entityNodeMap[entityId] = leafIndex;

    nodes[leafIndex].box      = box;
    nodes[leafIndex].entityId = entityId;
    nodes[leafIndex].isLeaf   = true;
    insert_node(leafIndex);
}

void bvh_tree::remove_leaf(u32 entityId)
{
    if(!entityNodeMap.contains(entityId))
    {
        logger::error("The entity map does not contain the entity with id: {}", entityId);
        return;
    }

    auto index = entityNodeMap[entityId];
    remove_node(index);
    deallocate_node(index);
    entityNodeMap.erase(entityId);
}

void bvh_tree::update_leaf(u32 entityId, const AABB& box)
{
    if(!entityNodeMap.contains(entityId))
    {
        logger::error("The entity map does not contain the entity with id: {}", entityId);
        return;
    }
    auto index = entityNodeMap[entityId];
    auto& n    = nodes[index];
    remove_node(index);
    n.box = box;
    insert_node(index);
}

std::list<u32> bvh_tree::query_tree(u32 entityId)
{
    std::list<u32> entities;
    auto current = entityNodeMap.find(entityId);
    if(current == entityNodeMap.end()) return entities;

    auto queriedIndex = current->second;
    auto& queriedNode = nodes[queriedIndex];

    std::stack<index_t> queriedNodes;
    queriedNodes.push(rootIndex);
    while(!queriedNodes.empty())
    {
        auto currentIndex = queriedNodes.top();
        auto& currentNode = nodes[currentIndex];
        queriedNodes.pop();

        if(!detail::intersects(currentNode.box, queriedNode.box)) continue;

        if(currentNode.isLeaf)
        {
            if(currentNode.entityId == entityId) continue;

            entities.push_back(currentNode.entityId);
        }
        else
        {
            queriedNodes.push(currentNode.childA);
            queriedNodes.push(currentNode.childB);
        }
    }
    return entities;
}

std::list<u32> bvh_tree::raycast(vec2 position)
{
    std::list<u32> entities;

    std::stack<index_t> queriedNodes;
    queriedNodes.push(rootIndex);
    while(!queriedNodes.empty())
    {
        auto currentIndex = queriedNodes.top();
        auto& currentNode = nodes[currentIndex];
        queriedNodes.pop();

        if(currentNode.box.min.x > position.x || currentNode.box.max.x < position.x
           || currentNode.box.min.y > position.y || currentNode.box.max.y < position.y)
            continue;

        if(currentNode.isLeaf)
        {
            entities.push_back(currentNode.entityId);
        }
        else
        {
            queriedNodes.push(currentNode.childA);
            queriedNodes.push(currentNode.childB);
        }
    }
    return entities;
}

void bvh_tree::insert_node(index_t leafIndex)
{
    auto& newNode      = nodes[leafIndex];
    auto& box          = newNode.box;
    const auto boxSize = box.max - box.min;
    box.min -= boxSize * fattenBy;
    box.max += boxSize * fattenBy;

    if(nodeCount == 1)
    {
        rootIndex = leafIndex;
        return;
    }

    // find the best sibling for the new leaf
    auto siblingIndex = find_sibling(box);

    // create a new parent
    auto oldParentIndex       = nodes[siblingIndex].parentIndex;
    auto newParentIndex       = allocate_node();
    auto& newParentNode       = nodes[newParentIndex];
    newParentNode.box         = detail::AABB_union(box, nodes[siblingIndex].box);
    newParentNode.parentIndex = oldParentIndex;
    newParentNode.isLeaf      = false;

    if(oldParentIndex != nullIndex)
    {
        if(nodes[oldParentIndex].childA == siblingIndex)
        {
            nodes[oldParentIndex].childA = newParentIndex;
        }
        else
        {
            nodes[oldParentIndex].childB = newParentIndex;
        }

        nodes[newParentIndex].childA    = siblingIndex;
        nodes[newParentIndex].childB    = leafIndex;
        nodes[siblingIndex].parentIndex = newParentIndex;
        nodes[leafIndex].parentIndex    = newParentIndex;
    }
    else
    {
        nodes[newParentIndex].childA    = siblingIndex;
        nodes[newParentIndex].childB    = leafIndex;
        nodes[siblingIndex].parentIndex = newParentIndex;
        nodes[leafIndex].parentIndex    = newParentIndex;
        rootIndex                       = newParentIndex;
    }

    // walk back up the tree refitting AABBs
    auto index = nodes[leafIndex].parentIndex;
    while(index != nullIndex)
    {
        index = rotate(index);

        auto& thisNode = nodes[index];
        auto& childA   = nodes[thisNode.childA];
        auto& childB   = nodes[thisNode.childB];

        thisNode.box    = detail::AABB_union(childA.box, childB.box);
        thisNode.height = 1 + std::max(childA.height, childB.height);

        index = thisNode.parentIndex;
    }
}

void bvh_tree::remove_node(index_t index)
{
    if(index == rootIndex)
    {
        rootIndex = nullIndex;
        return;
    }

    auto& currentNode     = nodes[index];
    auto parentIndex      = currentNode.parentIndex;
    auto& parentNode      = nodes[parentIndex];
    auto siblingIndex     = parentNode.childA == index ? parentNode.childA : parentNode.childB;
    auto& siblingNode     = nodes[siblingIndex];
    auto grandparentIndex = parentNode.parentIndex;
    if(grandparentIndex != nullIndex)
    {
        auto& grandparentNode = nodes[grandparentIndex];
        if(grandparentNode.childA == parentIndex)
        {
            grandparentNode.childA = siblingIndex;
        }
        else
        {
            grandparentNode.childB = siblingIndex;
        }
        siblingNode.parentIndex = grandparentIndex;
        deallocate_node(parentIndex);

        auto thisNodeIndex = grandparentIndex;
        while(thisNodeIndex != nullIndex)
        {
            thisNodeIndex  = rotate(thisNodeIndex);
            auto& thisNode = nodes[thisNodeIndex];
            auto& childA   = nodes[thisNode.childA];
            auto& childB   = nodes[thisNode.childB];

            thisNode.box    = detail::AABB_union(childA.box, childB.box);
            thisNode.height = 1 + std::max(childA.height, childB.height);

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

bvh_tree::index_t bvh_tree::rotate(index_t index)
{
    if(nodes[index].isLeaf || nodes[index].height < 2) return index;

    auto childAIndex = nodes[index].childA;
    auto childBIndex = nodes[index].childB;

    auto balance = nodes[childBIndex].height - nodes[childAIndex].height;
    if(balance > 1)
    {
        auto childBchildAIndex = nodes[childBIndex].childA;
        auto childBchildBIndex = nodes[childBIndex].childB;

        nodes[childBIndex].childA      = index;
        nodes[childBIndex].parentIndex = nodes[index].parentIndex;
        nodes[index].parentIndex       = childBIndex;

        if(nodes[childBIndex].parentIndex != nullIndex)
        {
            if(nodes[nodes[childBIndex].parentIndex].childA == index)
            {
                nodes[nodes[childBIndex].parentIndex].childA = childBIndex;
            }
            else
            {
                nodes[nodes[childBIndex].parentIndex].childB = childBIndex;
            }
        }
        else
        {
            rootIndex = childBIndex;
        }
        if(nodes[childBchildAIndex].height > nodes[childBchildBIndex].height)
        {
            nodes[childBIndex].childB            = childBchildAIndex;
            nodes[index].childB                  = childBchildBIndex;
            nodes[childBchildBIndex].parentIndex = index;
            nodes[index].box                     = detail::AABB_union(nodes[childAIndex].box, nodes[childBchildBIndex].box);
            nodes[childBIndex].box               = detail::AABB_union(nodes[index].box, nodes[childBchildAIndex].box);

            nodes[index].height       = 1 + std::max(nodes[childAIndex].height, nodes[childBchildBIndex].height);
            nodes[childBIndex].height = 1 + std::max(nodes[index].height, nodes[childBchildAIndex].height);
        }
        else
        {
            nodes[childBIndex].childB            = childBchildBIndex;
            nodes[index].childB                  = childBchildAIndex;
            nodes[childBchildAIndex].parentIndex = index;
            nodes[index].box                     = detail::AABB_union(nodes[childAIndex].box, nodes[childBchildAIndex].box);
            nodes[childBIndex].box               = detail::AABB_union(nodes[index].box, nodes[childBchildBIndex].box);

            nodes[index].height       = 1 + std::max(nodes[childAIndex].height, nodes[childBchildAIndex].height);
            nodes[childBIndex].height = 1 + std::max(nodes[index].height, nodes[childBchildBIndex].height);
        }
        return childBIndex;
    }
    else if(balance < -1)
    {
        auto childAchildAIndex = nodes[childAIndex].childA;
        auto childAchildBIndex = nodes[childAIndex].childB;

        nodes[childAIndex].childA      = index;
        nodes[childAIndex].parentIndex = nodes[index].parentIndex;
        nodes[index].parentIndex       = childAIndex;

        if(nodes[childAIndex].parentIndex != nullIndex)
        {
            if(nodes[nodes[childAIndex].parentIndex].childA == index)
            {
                nodes[nodes[childAIndex].parentIndex].childA = childAIndex;
            }
            else
            {
                nodes[nodes[childAIndex].parentIndex].childB = childAIndex;
            }
        }
        else
        {
            rootIndex = childAIndex;
        }
        if(nodes[childAchildAIndex].height > nodes[childAchildBIndex].height)
        {
            nodes[childAIndex].childB            = childAchildAIndex;
            nodes[index].childB                  = childAchildBIndex;
            nodes[childAchildBIndex].parentIndex = index;
            nodes[index].box                     = detail::AABB_union(nodes[childBIndex].box, nodes[childAchildBIndex].box);
            nodes[childAIndex].box               = detail::AABB_union(nodes[index].box, nodes[childAchildAIndex].box);

            nodes[index].height       = 1 + std::max(nodes[childBIndex].height, nodes[childAchildBIndex].height);
            nodes[childAIndex].height = 1 + std::max(nodes[index].height, nodes[childAchildAIndex].height);
        }
        else
        {
            nodes[childAIndex].childB            = childAchildBIndex;
            nodes[index].childB                  = childAchildAIndex;
            nodes[childAchildAIndex].parentIndex = index;
            nodes[index].box                     = detail::AABB_union(nodes[childBIndex].box, nodes[childAchildAIndex].box);
            nodes[childAIndex].box               = detail::AABB_union(nodes[index].box, nodes[childAchildBIndex].box);

            nodes[index].height       = 1 + std::max(nodes[childBIndex].height, nodes[childAchildAIndex].height);
            nodes[childAIndex].height = 1 + std::max(nodes[index].height, nodes[childAchildBIndex].height);
        }
        return childAIndex;
    }
    return index;
}

f32 bvh_tree::compute_cost()
{
    auto cost = 0.0f;
    for(auto& node : nodes)
    {
        if(!node.isLeaf && node.isActive)
            cost += detail::AABB_area(node.box);
    }
    return cost;
}

bvh_tree::index_t bvh_tree::find_sibling(const AABB& box)
{
    auto bestCost         = detail::AABB_area(detail::AABB_union(nodes[rootIndex].box, box));
    auto bestSiblingIndex = rootIndex;
    std::queue<index_t> bestCandidates;
    bestCandidates.push(rootIndex);

    auto inheritedCost = 0.f;
    while(!bestCandidates.empty())
    {
        auto currentIndex = bestCandidates.front();
        bestCandidates.pop();

        auto nodeCost       = detail::AABB_area(nodes[currentIndex].box);
        auto boxCost        = detail::AABB_area(box);
        auto directCost     = detail::AABB_area(detail::AABB_union(nodes[currentIndex].box, box));
        auto costOfChoosing = directCost + inheritedCost;
        inheritedCost += directCost - nodeCost;
        if(costOfChoosing <= bestCost)
        {
            bestCost         = costOfChoosing;
            bestSiblingIndex = currentIndex;
        }
        auto lowerBound = boxCost + inheritedCost;
        if(lowerBound < bestCost && !nodes[currentIndex].isLeaf)
        {
            bestCandidates.push(nodes[currentIndex].childA);
            bestCandidates.push(nodes[currentIndex].childB);
        }
    }
    return bestSiblingIndex;
}

bvh_tree::index_t bvh_tree::allocate_node()
{
    if(nextFree == nullIndex)
    {
        auto size = nodes.size();
        nodes.resize(size + 10);
        nextFree = nullIndex;
        for(int i = nodes.size() - 1; i >= int(size); --i)
        {
            nodes[i].nextFree = nextFree;
            nextFree          = i;
        }
    }
    auto newNodeIndex   = nextFree;
    auto& newNode       = nodes[newNodeIndex];
    newNode.parentIndex = nullIndex;
    newNode.childA      = nullIndex;
    newNode.childB      = nullIndex;
    newNode.height      = 0;
    newNode.isActive    = true;
    nodeCount += 1;
    nextFree = newNode.nextFree;
    return newNodeIndex;
}

void bvh_tree::deallocate_node(index_t index)
{
    auto& toDeallocate    = nodes[index];
    toDeallocate.nextFree = nextFree;
    toDeallocate.isActive = false;
    nextFree              = index;
    nodeCount -= 1;
}
}
