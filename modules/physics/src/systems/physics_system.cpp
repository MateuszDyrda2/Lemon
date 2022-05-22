#include <lemon/physics/systems/physics_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>

namespace lemon {
physics_system::physics_system(service_registry& globalRegistry):
    system(globalRegistry),
    clk(globalRegistry.get_service<game_clock>()),
    sch(globalRegistry.get_service<scheduler>())
{ }
physics_system::~physics_system()
{ }
void physics_system::on_update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    auto pdt = clk.get_physics_delta();

    auto view = registry.view<rigidbody>();
    // switch to a new state
    for(auto&& [ent, rb] : view.each())
    {
        rb._oldPosition        = rb.position;
        rb._oldRotation        = rb.rotation;
        rb._oldAngularVelocity = rb.angularVelocity;
        rb._oldVelocity        = rb.velocity;
    }

    sch.for_each(
        view.begin(), view.end(),
        [&](auto ent) {
            auto&& [rb] = view.get(ent);
            rb.velocity *= glm::clamp(1.f - rb.linearDrag * pdt, 0.f, 1.f);
            rb.angularVelocity *= glm::clamp(1.f - rb.angularDrag * pdt, 0.f, 1.f);
        });

    // auto group = registry.group<transform, collider, rigidbody>();
    auto trrb = registry.view<transform, rigidbody>();
    sch.for_each(
        trrb.begin(), trrb.end(),
        [&](auto ent) {
            auto&& [tr, rb] = trrb.get(ent);
            tr.position += rb.velocity * pdt;
            tr.rotation += rb.angularVelocity * pdt;
        });
    for(auto&& [ent, rb] : view.each())
    {
        registry.emplace_or_replace<dirty_t>(ent);
    }
}

#if 0 // not needed at the moment
f32 physics_system::calculate_inertia(const rigidbody& rb, const collider& coll)
{
    switch(coll.shape)
    {
    case collider::Box:
        return (1.f / 12.f) * rb.mass
               * ((2 * coll.box.hSize.x) * (2 * coll.box.hSize.x) + (2 * coll.box.hSize.y) * (2 * coll.box.hSize.y));
    case collider::Circle:
        return (0.5f) * rb.mass * (coll.circle.radius * coll.circle.radius);
    // https://www.gamedev.net/forums/topic/686143-calculating-the-moment-of-inertia-of-a-2d-capsule/5331404/
    case collider::Capsule:
        return (0.5f * rb.mass)
               * (9.f * (coll.capsule.radius * coll.capsule.radius)
                  + 4.f * (coll.capsule.hHeight * coll.capsule.hHeight)
                  + 1.5f * (coll.capsule.radius * coll.capsule.hHeight));
    }
}
#endif

}
