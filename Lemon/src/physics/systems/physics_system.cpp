#include <physics/systems/physics_system.h>

#include <core/math/math.h>
#include <physics/components/physics_components.h>
#include <world/components/transform_components.h>

namespace lemon {
physics_system::physics_system(scene& _scene, event_queue& _eventQueue):
    _scene(_scene)
{
    update = _eventQueue["Update"_hs] += [this](event_args* e) {
        this->onUpdate(e);
    };
}

physics_system::~physics_system() { }

void physics_system::onUpdate([[maybe_unused]] event_args* e)
{
    auto delta = static_cast<update_event*>(e)->deltaTime;
    auto view  = _scene.view<dirty_t, rigidbody, transform>();
    for(auto&& [_entity, _rigidbody, _transform] : view.each())
    {
        _rigidbody.position = _transform.position;
        _rigidbody.rotation = _transform.rotation;
    }

    auto rigidbodies = _scene.view<rigidbody>();
    for(auto&& [_entity, _rigidbody] : rigidbodies.each())
    {
        _rigidbody.velocity *= clamp(1.0f - _rigidbody.linearDrag * delta, 0.0f, 1.0f);
    }

    auto trrb = _scene.view<transform, rigidbody>();
    for(auto&& [_entity, _transform, _rigidbody] : trrb.each())
    {
        _transform.position += _rigidbody.velocity * delta;
        _rigidbody.position = _transform.position;
    }
}
}
