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
void physics_system::remove_from_tree(entity_registry&, entity_handle ent)
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
    f32 deltaTime = game::get_game_clock()->delta_time();
    for(auto&& [ent, rb] : registry.view<rigidbody>().each())
    {
        if(rb.bodyType == rigidbody::body_type::Dynamic)
        {
            pEngine.apply_gravity(rb, deltaTime);
        }
    }

    auto group = registry.group<rigidbody, collider>(entt::get<transform>);
    // Broad phase
    //*************************************************
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
                //                trns.position += collision->overlap * collision->axis;
                //                rb.velocity = { 0.f, 0.f };

                if(rb.bodyType == rigidbody::body_type::Dynamic)
                {
                    auto bounciness = std::min(coll.bounciness, otherColl.bounciness);
                    auto vj         = -(1 + bounciness) * dot(rb.velocity, collision->axis);
                    auto j          = vj;
                    rb.velocity += j * collision->axis;
                    registry.emplace_or_replace<dirty>(ent);
                }
                // TODO: Resolve collision
            }
            //*****************************************
        }
    }
    //*************************************************
    // update kinematics
    for(auto&& [ent, rb, coll, tr] : group.each())
    {
        pEngine.apply_drag(rb, deltaTime);
        pEngine.calculate_position(rb, tr.position, deltaTime);
        if(!rb.freezeRotation)
        {
            f32 inertia = pEngine.calculate_inertia(rb, coll);
            pEngine.calculate_rotation(
                rb, tr.rotation, inertia, deltaTime);
        }
        registry.emplace_or_replace<dirty>(ent);
    }
    //*************************************************

    // update Axis Aligned Bounding Boxes
    //*************************************************
    registry.view<dirty, const transform, const collider>().each([this](const entity_handle ent, const transform& tr, const collider& coll) {
        pEngine.update_collider(u32(ent), pEngine.get_AABB(coll, tr.position));
    });
    //*************************************************
}
void physics_system::move_entity(entity ent, const vec2& to)
{
    auto& trns  = ent.get_component<transform>();
    auto& rb    = ent.get_component<rigidbody>();
    rb.velocity = (to - trns.position) / game::get_game_clock()->delta_time();
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
}
void physics_system::add_torque(entity ent, f32 amount)
{
    auto& rb = ent.get_component<rigidbody>();
}
} // namespace lemon
