#include <lemon/engine/systems/physics_system.h>

#include <lemon/core/game.h>
#include <lemon/core/time/clock.h>
#include <lemon/scene/basic_components.h>
#include <lemon/scene/scene.h>

namespace lemon {
physics_system::physics_system(ptr<scene>)
{
}
physics_system::~physics_system()
{
}
void physics_system::update(entity_registry& registry)
{
    f32 deltaTime = game::get_game_clock()->delta_time();
    auto group    = registry.group<rigidbody, box_collider>(entt::get<transform>);
    for(auto&& [ent, rb, bc, tr] : group.each())
    {
        pEngine.apply_gravity(rb.force, rb.mass);
        pEngine.calculate_position(
            rb.force, rb.mass,
            rb.velocity, tr.position, deltaTime);
        f32 inertia = pEngine.calculate_inertia(0.08333f, rb.mass, bc.bounds);
        pEngine.calculate_rotation(
            rb.torque, inertia,
            rb.angularVelocity, tr.rotation, deltaTime);
        registry.emplace_or_replace<dirty>(ent);
    }
}
void physics_system::move_entity(entity ent, const vec2& to)
{
    auto& trns  = ent.get_component<transform>(ent);
    auto& rb    = ent.get_component<rigidbody>();
    rb.velocity = (to - trns.position) / deltaTime;
}
void physics_system::rotate_entity(entity ent, f32 to)
{
    auto& trns = ent.get_component<transform>();
    auto& rb   = ent.get_component<rigidbody>();
    rb.torque  = (to - trns.rotation) / deltaTime;
}
void physics_system::add_force(entity ent, const vec2& amount)
{
    auto& rb = ent.get_component<rigidbody>();
    rb.force += amount;
}
void physics_system::add_torque(entity ent, f32 amount)
{
    auto& rb = ent.get_component<rigidbody>();
    rb.torque += amount;
}
} // namespace lemon
