#pragma once

#include "../bvh_tree.h"
#include "../detail/collision_set.h"
#include "../sat_algorithm.h"

#include <core/math/vec2.h>
#include <events/events.h>
#include <world/entity.h>
#include <world/service_container.h>
#include <world/system.h>

#include <optional>

namespace lemon {
struct rigidbody;
struct box_collider;
struct circle_collider;
struct capsule_collider;
class LEMON_API collision_system : public system
{
  public:
    SYSTEM(collision_system, scene, event_queue);
    ~collision_system();
    void onUpdate(event_args* e);

  private:
    bvh_tree tree;
    sat_algorithm narrowPhase;
    scene& _scene;
    event_queue& _eventQueue;
    collision_set set;
    event_queue::listener_handle update;

  private:
    std::optional<detail::AABB> get_aabb(
        const entity_t _entity, const rigidbody& _rigidbody);
    void collision_events(entity_t a, entity_t b);
    void physics_reponse(
        f32 fixedDelta,
        f32 bounciness,
        const mtv& vec,
        rigidbody& aRigidbody,
        rigidbody& bRigidbody);
    void handle_box_collisions(
        f32 fixedDelta,
        collision_set& newSet,
        entity_t a,
        rigidbody& rb,
        const box_collider& collider,
        const std::list<u32>& collisions);
    void handle_circle_collisions(
        f32 fixedDelta,
        collision_set& newSet,
        entity_t a,
        rigidbody& rb,
        const circle_collider& collider,
        const std::list<u32>& collisions);
    void handle_capsule_collisions(
        f32 fixedDelta,
        collision_set& newSet,
        entity_t a,
        rigidbody& rb,
        const capsule_collider& collider,
        const std::list<u32>& collisions);
    std::optional<mtv> box_box_collision(
        const rigidbody& aRigidbody, const box_collider& aCollider,
        const rigidbody& bRigidbody, const box_collider& bCollider);
    std::optional<mtv> box_circle_collision(
        const rigidbody& aRigidbody, const box_collider& aCollider,
        const rigidbody& bRigidbody, const circle_collider& bCollider);
    std::optional<mtv> box_capsule_collision(
        const rigidbody& aRigidbody, const box_collider& aCollider,
        const rigidbody& bRigidbody, const capsule_collider& bCollider);
    std::optional<mtv> circle_circle_collision(
        const rigidbody& aRigidbody, const circle_collider& aCollider,
        const rigidbody& bRigidbody, const circle_collider& bCollider);
    std::optional<mtv> circle_capsule_collision(
        const rigidbody& aRigidbody, const circle_collider& aCollider,
        const rigidbody& bRigidbody, const capsule_collider& bCollider);
    std::optional<mtv> capsule_capsule_collision(
        const rigidbody& aRigidbody, const capsule_collider& aCollider,
        const rigidbody& bRigidbody, const capsule_collider& bCollider);
};
}
