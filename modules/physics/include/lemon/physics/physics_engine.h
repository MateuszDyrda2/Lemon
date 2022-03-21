#pragma once

#include "BVH_tree.h"
#include "SAT.h"
#include "physics_components.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec4.h>

namespace lemon {
class LEMON_PUBLIC physics_engine
{
  public:
    physics_engine();
    ~physics_engine();
    void apply_gravity(rigidbody& rb, f32 deltaTime);
    void calculate_position(rigidbody& rb, vec2& pos, f32 deltaTime);
    void calculate_rotation(rigidbody& rb, f32& rotation, f32 inertia, f32 deltaTime);
    f32 calculate_inertia(const rigidbody& rb, const collider& coll);
    AABB get_AABB(const collider& coll, const vec2& position) const noexcept;

    void insert_collider(u32 entityId, const AABB& box);
    void remove_collider(u32 entityId);
    void update_collider(u32 entityId, const AABB& box);
    std::list<u32> broad_collisions(u32 entityId);
    std::optional<MTV> collide(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation)
        const noexcept;

  private:
    f32 gravity;
    BVH_tree tree;
    SAT npAlgorithm;

  private:
    std::optional<MTV> box_box_collision(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept;
    std::optional<MTV> box_circle_collision(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept;
    std::optional<MTV> box_capsule_collision(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept;
    std::optional<MTV> circle_circle_collision(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept;
    std::optional<MTV> circle_capsule_collision(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept;
    std::optional<MTV> capsule_capsule_collision(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept;

    void rotate_box(vec2& a, vec2& b, vec2& c, vec2& d, f32 degrees) const noexcept;
};
} // namespace lemon
