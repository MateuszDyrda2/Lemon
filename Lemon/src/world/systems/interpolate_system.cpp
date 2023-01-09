#include "physics/components/physics_components.h"
#include "world/components/transform_components.h"
#include <world/systems/interpolate_system.h>

namespace lemon {
interpolate_system::interpolate_system(scene& _scene, event_queue& _eventQueue):
    _scene(_scene)
{
    update = _eventQueue["Update"_hs] += [this](event_args* e) {
        this->on_update(e);
    };
}

interpolate_system::~interpolate_system() { }

void interpolate_system::on_update(event_args* e)
{
    auto&& [delta, alpha] = get_event<update_event>(e);
    auto omalpha          = 1.0f - alpha;

    auto trrb = _scene.group<transform, rigidbody>();
    for (auto&& [_entity, _transform, _rigidbody] : trrb.each())
    {
        _transform.position = _rigidbody.position * alpha
                              + _rigidbody._oldPosition * omalpha;
    }
}

}
