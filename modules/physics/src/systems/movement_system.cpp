#include <lemon/physics/systems/movement_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/core/math/math.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>
#include <lemon/scene/scene.h>

namespace lemon {
movement_system::movement_system(ptr<scene>, clock& clk, scheduler& sch):
    sch(sch), clk(clk)
{
}
movement_system::~movement_system()
{
}
void movement_system::update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    auto deltaTime = clk.delta_time();

    auto view = registry.view<rigidbody>();
    sch.for_each(view.begin(), view.end(),
                 [&](auto ent) {
                     auto&& [rb] = view.get(ent);
                     rb.velocity *= clamp(1.f - rb.linearDrag * deltaTime, 0.f, 1.f);
                     rb.angularVelocity *= clamp(1.f - rb.angularDrag * deltaTime, 0.f, 1.f);
                 });

    // auto group = registry.group<transform, collider, rigidbody>();
    auto trrb = registry.view<transform, rigidbody>();
    sch.for_each(trrb.begin(), trrb.end(),
                 [&](auto ent) {
                     auto&& [tr, rb] = trrb.get(ent);
                     tr.position += rb.velocity * deltaTime;
                     tr.rotation += rb.angularVelocity * deltaTime;
                 });
}

} // lemon
