#pragma once

#include <lemon/core/math/vec4.h>
#include <lemon/physics/BVH_tree.h>
#include <lemon/physics/SAT.h>
#include <lemon/scene/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>
#include <lemon/scene/system.h>

namespace lemon {
class scene;
class LEMON_PUBLIC physics_system : public system
{
  public:
    physics_system(ptr<scene> s);
    ~physics_system();

    void update(entity_registry& registry) override;

    static void move_entity(entity ent, const vec2& to);
    static void rotate_entity(entity ent, f32 to);
    static void add_force(entity ent, const vec2& amount);
    static void add_torque(entity ent, f32 amount);

  private:
    f32 gravity;
    BVH_tree tree;
    SAT npAlgorithm;

  private:
    void add2tree(entity_registry& registry, entity_handle ent);
    void remove_from_tree(entity_registry& registry, entity_handle ent);

    AABB get_AABB(const collider& col, const transform& tr);
    f32 calculate_inertia(const rigidbody& rb, const collider& coll);

    void apply_gravity(rigidbody& rb, f32 deltaTime);
    void apply_drag(rigidbody& rb, f32 deltaTime);
    void calculate_position(const rigidbody& rb, transform& tr, f32 deltaTime);
    void calculate_rotation(const rigidbody& rb, transform& tr, f32 inertia, f32 deltaTime);

    std::list<u32> broad_collisions(u32 entityId);
    std::optional<MTV> collide(
        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation)
        const noexcept;
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