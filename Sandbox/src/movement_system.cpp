#include "movement_system.h"
#include "physics/systems/physics_system.h"
#include "player_components.h"
#include "rendering/components/rendering_components.h"
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
    currentDirection = FORWARD;
}

movement_system::~movement_system() { }

void movement_system::onUpdate([[maybe_unused]] event_args* e)
{
    f32 speed     = 300.f;
    auto velocity = vec2{
        _input.check_axis({ keycode::a, keycode::d }),
        _input.check_axis({ keycode::s, keycode::w }),
    };
    velocity = normalize(velocity);

    auto player = _scene.get_entity(_scene.view<player_t>().front());

    auto newDirection = get_direction(velocity);
    if (currentDirection != newDirection)
    {
        switch (newDirection)
        {
        case NONE:
            player.emplace<start_animation_m>("idle"_hs);
            break;
        case LEFT:
            player.emplace<start_animation_m>("left"_hs);
            break;
        case RIGHT:
            player.emplace<start_animation_m>("right"_hs);
            break;
        case BACK:
            player.emplace<start_animation_m>("back"_hs);
            break;
        case FORWARD:
            player.emplace<start_animation_m>("forward"_hs);
            break;
        case LEFT_BACK:
            player.emplace<start_animation_m>("left_back"_hs);
            break;
        case RIGHT_BACK:
            player.emplace<start_animation_m>("right_back"_hs);
            break;
        case LEFT_FORWARD:
            player.emplace<start_animation_m>("left_forward"_hs);
            break;
        case RIGHT_FORWARD:
            player.emplace<start_animation_m>("right_forward"_hs);
            break;
        }
        currentDirection = newDirection;
    }

    physics_system::set_velocity(player, velocity * speed);
}

movement_system::direction
movement_system::get_direction(const vec2& velocity)
{
    if (velocity.x > 0.f)
    {
        if (velocity.y > 0.f)
            return direction::RIGHT_FORWARD;
        else if (velocity.y < 0.f)
            return direction::RIGHT_BACK;
        else
            return direction::RIGHT;
    }
    else if (velocity.x < 0.f)
    {
        if (velocity.y > 0.f)
            return direction::LEFT_FORWARD;
        else if (velocity.y < 0.f)
            return direction::LEFT_BACK;
        else
            return direction::LEFT;
    }
    else
    {
        if (velocity.y > 0.f)
            return direction::FORWARD;
        else if (velocity.y < 0.f)
            return direction::BACK;
        else
            return direction::NONE;
    }
}
