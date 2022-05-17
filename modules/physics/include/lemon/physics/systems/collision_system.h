#pragma once

#include "../BVH_tree.h"
#include "../SAT.h"

#include <lemon/scene/system.h>
#include <lemon/threading/scheduler.h>

// TODO: Implement sleeping when velocity is ~= 0 for more than a frame
// TODO: Implement collision caching to send correct collisionEnter/Leave events
// TODO: use different tag / store aabbs somewhere
// TODO: use sweep and prune for rigidbodies
namespace lemon {
class scene;
class LEMON_PUBLIC collision_system : public system
{
  public:
    collision_system(ptr<scene> s, scheduler& sch);
    ~collision_system();

    void update(entity_registry& registry) override;

  private:
    scheduler& sch;
    BVH_tree tree;
    SAT npAlgorithm;

  private:
    void add2tree(entity_registry& registry, entity_handle ent);
    void remove_from_tree(entity_registry& registy, entity_handle ent);
};
} // namespace lemon
