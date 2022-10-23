#include "movement_system.h"
#include "physics/systems/physics_system.h"
#include "player_components.h"
#include "world/components/transform_components.h"
#include "world/systems/transform_system.h"
#include <physics/components/physics_components.h>

using namespace lemon;
movement_system::movement_system(scene& _scene,
                                 event_queue& _eventQueue,
                                 input& _input):
    _input(_input),
    _scene(_scene)
{
    /*    f32 speed  = 200.f;*/
    /*horizontal = _eventQueue["Horizontal"_hs] += [&, speed](event_args* e) {*/
    /*auto&& [value]                     = get_event<axis_event>(e);*/
    /*auto player                        = _scene.get_entity(_scene.view<player_t>().front());*/
    /*player.get<rigidbody>().velocity.x = value * speed;*/
    /*};*/

    /*vertical = _eventQueue["Vertical"_hs] += [&, speed](event_args* e) {*/
    /*auto&& [value]                     = get_event<axis_event>(e);*/
    /*auto player                        = _scene.get_entity(_scene.view<player_t>().front());*/
    /*player.get<rigidbody>().velocity.y = value * speed;*/
    /*};*/

    update = _eventQueue["PhysicsUpdate"_hs] += [this](event_args* e) {
        this->onUpdate(e);
    };
}

movement_system::~movement_system() { }

void movement_system::onUpdate([[maybe_unused]] event_args* e)
{
    f32 speed     = 500.f;
    auto velocity = vec2{
        _input.check_axis({ keycode::a, keycode::d }),
        _input.check_axis({ keycode::s, keycode::w }),
    };
    velocity = normalize(velocity);

    auto player = _scene.get_entity(_scene.view<player_t>().front());
    physics_system::set_velocity(player, velocity * speed);
}
