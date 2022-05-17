#include <lemon/physics/systems/physics_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>

namespace lemon {
physics_system::physics_system(ptr<scene> /*s*/, clock& clk, scheduler& sch):
    clk(clk), sch(sch)
{ }
physics_system::~physics_system()
{ }
void physics_system::update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    auto deltaTime = clk.delta_time();

    auto view = registry.view<rigidbody>();
    sch.for_each(view.begin(), view.end(),
                 [&](auto ent) {
                     auto&& [rb] = view.get(ent);
                     rb.velocity *= glm::clamp(1.f - rb.linearDrag * deltaTime, 0.f, 1.f);
                     rb.angularVelocity *= glm::clamp(1.f - rb.angularDrag * deltaTime, 0.f, 1.f);
                 });

    // auto group = registry.group<transform, collider, rigidbody>();
    auto trrb = registry.view<transform, rigidbody>();
    sch.for_each(
        trrb.begin(), trrb.end(),
        [&](auto ent) {
            auto&& [tr, rb] = trrb.get(ent);
            tr.position += rb.velocity * deltaTime;
            tr.rotation += rb.angularVelocity * deltaTime;
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
