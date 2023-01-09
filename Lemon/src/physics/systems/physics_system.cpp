#include "world/components/entity_components.h"
#include <physics/systems/physics_system.h>

#include <core/math/math.h>
#include <physics/components/physics_components.h>
#include <type_traits>
#include <world/components/transform_components.h>

namespace lemon {
physics_system::physics_system(scene& _scene, event_queue& _eventQueue):
    _scene(_scene)
{
    update = _eventQueue["PhysicsUpdate"_hs] += [this](event_args* e) {
        this->on_update(e);
    };
}

physics_system::~physics_system() { }

void physics_system::on_update([[maybe_unused]] event_args* e)
{
    auto rigidbodies = _scene.view<rigidbody, enabled_t>();

    auto trrb = _scene.group<transform, rigidbody>(entt::get_t<enabled_t>());
    for (auto&& [_entity, _transform, _rigidbody] : trrb.each())
    {
        _rigidbody.position = _transform.position;
        _rigidbody.rotation = _transform.rotation;
    }

    // save previous state
    for (auto&& [_entity, _rigidbody] : rigidbodies.each())
    {
        _rigidbody._oldPosition = _rigidbody.position;
    }

    auto fixedDelta = get_event<fixed_update_event>(e).fixedDelta;
    for (auto&& [_entity, _rigidbody] : rigidbodies.each())
    {
        _rigidbody.velocity *= clamp(1.0f - _rigidbody.linearDrag * fixedDelta, 0.0f, 1.0f);
    }

    static constexpr auto CutoffValue = 0.0001f;
    for (auto&& [_entity, _rigidbody] : rigidbodies.each())
    {
        if (in_range(_rigidbody.velocity.x, -CutoffValue, CutoffValue)
            && in_range(_rigidbody.velocity.y, -CutoffValue, CutoffValue))
            continue;

        _rigidbody.position += _rigidbody.velocity * fixedDelta;
        entity(_scene, _entity).emplace_or_replace<dirty_t>();
    }
}

void physics_system::set_velocity(entity _entity, vec2 newVelocity)
{
    lemon_assert(_entity.has<rigidbody>());
    _entity.get<rigidbody>().velocity = newVelocity;
}

void physics_system::add_velocity(entity _entity, vec2 velocity)
{
    lemon_assert(_entity.has<rigidbody>());
    _entity.get<rigidbody>().velocity += velocity;
}

void physics_system::move_position(entity _entity, vec2 newPosition, f32 deltaTime)
{
    lemon_assert(_entity.has<rigidbody>());
    lemon_assert(_entity.has<transform>());
    auto&& [rb, tr] = _entity.get<rigidbody, transform>();
    auto& oldPos    = tr.position;
    rb.velocity     = (newPosition - oldPos) / deltaTime;
}
}
