#include <lemon/engine/systems/physics_system.h>

#include <lemon/core/game.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/time/clock.h>
#include <lemon/scene/basic_components.h>
#include <lemon/scene/scene.h>

#include <list>
#include <utility>

namespace lemon {
void physics_system::add2tree(entity_registry& registry, entity_handle ent)
{
    const transform& tr  = registry.get<const transform>(ent);
    const collider& coll = registry.get<const collider>(ent);
    pEngine.insert_collider(u32(ent), pEngine.get_AABB(coll, tr.position));
}
void physics_system::remove_from_tree(entity_registry& registry, entity_handle ent)
{
    pEngine.remove_collider(u32(ent));
}
physics_system::physics_system(ptr<scene> s)
{
    s->get_registry()
        .on_construct<collider>()
        .connect<&physics_system::add2tree>(this);
    s->get_registry()
        .on_destroy<collider>()
        .connect<&physics_system::remove_from_tree>(this);
}
physics_system::~physics_system()
{
}
void physics_system::update(entity_registry& registry)
{
    // update Axis Aligned Bounding Boxes
    //*************************************************
    registry.view<dirty, const transform, const collider>().each([this](const entity_handle ent, const transform& tr, const collider& coll) {
        pEngine.update_collider(u32(ent), pEngine.get_AABB(coll, tr.position));
    });
    //*************************************************

    // Broad phase
    //*************************************************
    auto group = registry.group<rigidbody, collider>(entt::get<transform>);
    for(auto&& [ent, rb, coll, trns] : group.each())
    {
        for(const auto& other : pEngine.broad_collisions(u32(ent)))
        {
            // Narrow phase
            //*****************************************
            const auto&& [otherTrns, otherColl] = registry.get<const transform, const collider>(entity_handle(other));
            if(auto collision = pEngine.collide(
                   coll, trns.position, trns.rotation,
                   otherColl, otherTrns.position, otherTrns.rotation))
            {
                // TODO: Send message on collision
                // TODO: Resolve collision
            }
            //*****************************************
        }
    }

    //*************************************************
    // update kinematics
    f32 deltaTime = game::get_game_clock()->delta_time();
    for(auto&& [ent, rb, coll, tr] : group.each())
    {
        pEngine.apply_gravity(rb);
        pEngine.calculate_position(
            rb, tr.position, deltaTime);

        f32 inertia = pEngine.calculate_inertia(rb, coll);
        pEngine.calculate_rotation(
            rb, tr.rotation, inertia, deltaTime);
        registry.emplace_or_replace<dirty>(ent);
    }
    //*************************************************
}
void physics_system::move_entity(entity ent, const vec2& to)
{
    auto& trns  = ent.get_component<transform>();
    auto& rb    = ent.get_component<rigidbody>();
    rb.velocity = (to - trns.position) / game::get_game_clock()->delta_time();
    ;
}
void physics_system::rotate_entity(entity ent, f32 to)
{
    auto& trns         = ent.get_component<transform>();
    auto& rb           = ent.get_component<rigidbody>();
    rb.angularVelocity = (to - trns.rotation) / game::get_game_clock()->delta_time();
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
