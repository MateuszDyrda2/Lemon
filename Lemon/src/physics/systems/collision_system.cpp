#include "core/math/mat2.h"
#include "core/math/mat4.h"
#include "core/math/math.h"
#include <algorithm>
#include <cmath>
#include <physics/systems/collision_system.h>

#include <physics/components/physics_components.h>
#include <type_traits>
#include <world/components/transform_components.h>
#include <world/system.h>

namespace lemon {
std::optional<detail::AABB>
collision_system::get_aabb(const entity_t _entity, const rigidbody& _rigidbody)
{
    switch (_rigidbody.colliderType)
    {
    case collider_type::box:
    {
        auto& box = _scene.get<box_collider>(_entity);
        return detail::AABB{
            .min = { _rigidbody.position + box.offset - box.hSize },
            .max = { _rigidbody.position + box.offset + box.hSize },
        };
    }
    case collider_type::circle:
    {
        auto& circle = _scene.get<circle_collider>(_entity);
        return detail::AABB{
            .min = { _rigidbody.position + circle.offset - circle.radius },
            .max = { _rigidbody.position + circle.offset + circle.radius },
        };
    }
    default:
        return {};
    }
}

collision_system::collision_system(scene& _scene, event_queue& _eventQueue):
    _scene(_scene), _eventQueue(_eventQueue)
{
    update = _eventQueue["PhysicsUpdate"_hs] += [this](event_args* e) {
        this->onUpdate(e);
    };
}

collision_system::~collision_system() { }

void collision_system::onUpdate([[maybe_unused]] event_args* e)
{
    collision_set newSet;
    auto&& [fixedDelta] = get_event<fixed_update_event>(e);

    for (auto&& [_entity, _rigidbody] : _scene.view<rigidbody, dirty_t>().each())
    {
        if (auto aabb = get_aabb(_entity, _rigidbody))
            tree.update_leaf(std::underlying_type_t<typeof(_entity)>(_entity), *aabb);
    }

    for (auto&& [_entity, _rigidbody] : _scene.view<rigidbody>().each())
    {
        auto collisions = tree.query_tree(u32(_entity));
        switch (_rigidbody.colliderType)
        {
        case collider_type::box:
        {
            auto _box = _scene.get<box_collider>(_entity);
            handle_box_collisions(fixedDelta, newSet, _entity, _rigidbody, _box, collisions);
        }
        break;
        case collider_type::circle:
        {
            auto _circle = _scene.get<circle_collider>(_entity);
            handle_circle_collisions(fixedDelta, newSet, _entity, _rigidbody, _circle, collisions);
        }
        break;
        case collider_type::capsule:
        {
            auto _capsule = _scene.get<capsule_collider>(_entity);
            handle_capsule_collisions(fixedDelta, newSet, _entity, _rigidbody, _capsule, collisions);
        }
        break;
        default:
            break;
        }
    }

    // clear collisions
    set.diff(newSet, [&](entity_t a, entity_t b) {
        _eventQueue["CollisionEnded"_hs].fire(new detail::collision_event(a, b));
    });
    set.swap(newSet);
}

void collision_system::collision_events(entity_t a, entity_t b)
{
    if (set.has(a, b))
    {
        // collision repeated
        _eventQueue["CollisionRepeated"_hs].fire(
            new detail::collision_event(a, b));
    }
    else
    {
        // collision started
        _eventQueue["CollisionStarted"_hs].fire(
            new detail::collision_event(a, b));
    }
}

void collision_system::physics_reponse(f32 fixedDelta,
                                       f32 bounciness, const mtv& vec,
                                       rigidbody& aRigidbody, rigidbody& bRigidbody)
{
    // J = (-(1 + e)Vab * n) / (n * n(1/ma + 1/mb))
    // Va+ = Va- + J/ma * n
    // Vb+ = Vb- - J/mb * n
    const auto oma = i32(aRigidbody.isKinetic) * 1.f / aRigidbody.mass;
    const auto omb = i32(bRigidbody.isKinetic) * 1.f / bRigidbody.mass;

    const auto Vab  = (aRigidbody.velocity - bRigidbody.velocity);
    const auto rVel = glm::dot(Vab, vec.axis);
    const auto J    = (-(1 + bounciness) * rVel) / (oma + omb);

    const auto Va = aRigidbody.velocity + J * oma * vec.axis;
    const auto Vb = aRigidbody.velocity - J * omb * vec.axis;

    aRigidbody.velocity = Va;
    bRigidbody.velocity = Vb;

    aRigidbody.position = aRigidbody.velocity * fixedDelta;
    bRigidbody.position = bRigidbody.velocity * fixedDelta;
}

void collision_system::handle_box_collisions(
    f32 fixedDelta,
    collision_set& newSet,
    entity_t a,
    rigidbody& rb,
    const box_collider& collider,
    const std::list<u32>& collisions)
{

    for (const auto& other : collisions)
    {
        auto otherEntity      = _scene.get_entity(entity_t(other));
        auto&& otherRigidbody = otherEntity.get<rigidbody>();

        if (otherEntity.has<box_collider>())
        {
            // box x box collision
            auto&& otherBox = otherEntity.get<box_collider>();
            if (auto coll = box_box_collision(rb, collider, otherRigidbody, otherBox))
            {
                if (!newSet.push(a, entity_t(other))) continue;
                collision_events(a, entity_t(other));
                physics_reponse(
                    fixedDelta, std::min(collider.bounciness, otherBox.bounciness),
                    *coll, rb, otherRigidbody);
            }
        }
        else if (otherEntity.has<circle_collider>())
        {
            // box x circle collision
            auto&& otherCircle = otherEntity.get<circle_collider>();
            if (auto coll = box_circle_collision(rb, collider, otherRigidbody, otherCircle))
            {
                if (!newSet.push(a, entity_t(other))) continue;
                collision_events(a, entity_t(other));

                physics_reponse(
                    fixedDelta, std::min(collider.bounciness, otherCircle.bounciness),
                    *coll, rb, otherRigidbody);
            }
        }
        else if (otherEntity.has<capsule_collider>())
        {
            // box x capsule collision
        }
    }
}

void collision_system::handle_circle_collisions(
    f32 fixedDelta,
    collision_set& newSet,
    entity_t a,
    rigidbody& rb,
    const circle_collider& collider,
    const std::list<u32>& collisions)
{
    for (const auto& other : collisions)
    {
        auto otherEntity      = _scene.get_entity(entity_t(other));
        auto&& otherRigidbody = otherEntity.get<rigidbody>();

        if (otherEntity.has<box_collider>())
        {
            // circle x box collision
            auto&& otherBox = otherEntity.get<box_collider>();
            if (auto coll = box_circle_collision(otherRigidbody, otherBox, rb, collider))
            {
                if (!newSet.push(a, entity_t(other))) continue;
                collision_events(a, entity_t(other));
                physics_reponse(
                    fixedDelta,
                    std::min(collider.bounciness, otherBox.bounciness),
                    *coll, rb, otherRigidbody);
            }
        }
        else if (otherEntity.has<circle_collider>())
        {
            // circle x circle collision
            auto&& otherCircle = otherEntity.get<circle_collider>();
            if (auto coll = circle_circle_collision(rb, collider, otherRigidbody, otherCircle))
            {
                if (!newSet.push(a, entity_t(other))) continue;
                collision_events(a, entity_t(other));
                physics_reponse(
                    fixedDelta,
                    std::min(collider.bounciness, otherCircle.bounciness),
                    *coll, rb, otherRigidbody);
            }
        }
        else if (otherEntity.has<capsule_collider>())
        {
            // circle x capsule collision
        }
    }
}

void collision_system::handle_capsule_collisions(
    [[maybe_unused]] f32 fixedDelta,
    [[maybe_unused]] collision_set& newSet,
    [[maybe_unused]] entity_t a,
    [[maybe_unused]] rigidbody& rb,
    [[maybe_unused]] const capsule_collider& collider,
    [[maybe_unused]] const std::list<u32>& collisions)
{
    for (const auto& other : collisions)
    {
        auto otherEntity                       = _scene.get_entity(entity_t(other));
        [[maybe_unused]] auto&& otherRigidbody = otherEntity.get<rigidbody>();

        if (otherEntity.has<box_collider>())
        {
            // capsule x box collision
        }
        else if (otherEntity.has<circle_collider>())
        {
            // capsule x circle collision
        }
        else if (otherEntity.has<capsule_collider>())
        {
            // capsule x capsule collision
        }
    }
}

std::optional<mtv> collision_system::box_box_collision(
    const rigidbody& aRigidbody, const box_collider& aCollider,
    const rigidbody& bRigidbody, const box_collider& bCollider)
{
    auto aBox = create_box(
        aCollider.hSize,
        aRigidbody.position + aCollider.offset,
        aRigidbody.rotation);
    auto bBox = create_box(
        bCollider.hSize,
        bRigidbody.position + bCollider.offset,
        bRigidbody.rotation);

    return narrowPhase(aBox, bBox);
}

std::optional<mtv> collision_system::box_circle_collision(
    const rigidbody& aRigidbody, const box_collider& aCollider,
    const rigidbody& bRigidbody, const circle_collider& bCollider)
{
    auto aBox = create_box(
        aCollider.hSize,
        aRigidbody.position + aCollider.offset,
        aRigidbody.rotation);
    auto bCircle = create_circle(
        bCollider.radius,
        bRigidbody.position + bCollider.offset);

    return narrowPhase(aBox, bCircle);
}

std::optional<mtv> collision_system::box_capsule_collision(
    [[maybe_unused]] const rigidbody& aRigidbody, [[maybe_unused]] const box_collider& aCollider,
    [[maybe_unused]] const rigidbody& bRigidbody, [[maybe_unused]] const capsule_collider& bCollider)
{
    return {};
}

std::optional<mtv> collision_system::circle_circle_collision(
    const rigidbody& aRigidbody, const circle_collider& aCollider,
    const rigidbody& bRigidbody, const circle_collider& bCollider)
{
    auto aCircle = create_circle(
        aCollider.radius,
        aRigidbody.position + aCollider.offset);
    auto bCircle = create_circle(
        bCollider.radius,
        bRigidbody.position + bCollider.offset);

    return narrowPhase(aCircle, bCircle);
}

std::optional<mtv> collision_system::circle_capsule_collision(
    [[maybe_unused]] const rigidbody& aRigidbody, [[maybe_unused]] const circle_collider& aCollider,
    [[maybe_unused]] const rigidbody& bRigidbody, [[maybe_unused]] const capsule_collider& bCollider)
{
    return {};
}

std::optional<mtv> collision_system::capsule_capsule_collision(
    [[maybe_unused]] const rigidbody& aRigidbody, [[maybe_unused]] const capsule_collider& aCollider,
    [[maybe_unused]] const rigidbody& bRigidbody, [[maybe_unused]] const capsule_collider& bCollider)
{
    return {};
}
}
