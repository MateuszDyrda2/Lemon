#pragma once

#include <lemon/core/time/clock.h>
#include <lemon/scene/scene.h>
#include <lemon/threading/scheduler.h>

#include <lemon/physics/BVH_tree.h>
#include <lemon/physics/SAT.h>

// TODO: Implement sleeping when velocity is ~= 0 for more than a frame
// TODO: Implement collision caching to send correct collisionEnter/Leave events
namespace lemon {
class scene;
struct collision
{
    MTV mtv;
    entity_handle A;
    entity_handle B;
};
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

  private:
    std::optional<MTV> collide(
        const collider& lhs, const transform& lhsTr,
        const collider& rhs, const transform& rhsTr) noexcept;
    std::optional<MTV> box_box_collision(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) noexcept;
    std::optional<MTV> box_circle_collision(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) noexcept;
    std::optional<MTV> circle_circle_collision(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) noexcept;
};
} // namespace lemon
