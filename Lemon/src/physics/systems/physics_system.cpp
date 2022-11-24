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
        this->onUpdate(e);
    };
}

physics_system::~physics_system() { }

void physics_system::onUpdate([[maybe_unused]] event_args* e)
{
    auto rigidbodies = _scene.view<rigidbody>();

    auto trrb = _scene.group<transform, rigidbody>();
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

void physics_system::set_velocity(entity& _entity, vec2 newVelocity)
{
    _entity.get<rigidbody>().velocity = newVelocity;
}

void physics_system::add_velocity(entity& _entity, vec2 velocity)
{
    _entity.get<rigidbody>().velocity += velocity;
}

void physics_system::move_position(entity& _entity, vec2 newPosition)
{
    (void)_entity;
    (void)newPosition;
}
}
