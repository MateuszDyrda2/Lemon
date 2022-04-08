#include <lemon/engine/systems/movement_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/core/math/math.h>
#include <lemon/scene/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>
#include <lemon/scene/scene.h>

namespace lemon {
movement_system::movement_system(ptr<scene> s, clock& clk, scheduler& sch):
    clk(clk), sch(sch)
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
                 [&, this](auto ent, auto& rb) {
                     rb.velocity *= clamp(1.f - rb.linearDrag * deltaTime, 0.f, 1.f);
                     rb.angularVelocity *= clamp(1.f - rb.angularDrag * deltaTime, 0.f, 1.f);
                 });

    auto group = registry.group<transform, collider, rigidbody>();
    sch.for_each(group.begin(), group.end(),
                 [&, this](auto ent, auto& tr, auto& coll, auto& rb) {
                     tr.position += rb.velocity * deltaTime;
                     tr.rotation += rb.angularVelocity * deltaTime;
                 });
}

} // lemon