#pragma once

#include <lemon/core/time/clock.h>
#include <lemon/scene/system.h>
#include <lemon/threading/scheduler.h>

#include <lemon/physics/BVH_tree.h>
#include <lemon/physics/SAT.h>

// TODO: Implement sleeping when velocity is ~= 0 for more than a frame
// TODO: Implement collision caching to send correct collisionEnter/Leave events
// TODO: use different tag / store aabbs somewhere
// TODO: use sweep and prune for rigidbodies
namespace lemon {
class scene;
class LEMON_PUBLIC collision_system : public system
{
  public:
    collision_system(ptr<scene> s, clock& clk, scheduler& sch);
    ~collision_system();

    void update(entity_registry& registry) override;

  private:
    clock& clk;
    scheduler& sch;
    BVH_tree tree;
    SAT npAlgorithm;
};
} // namespace lemon
