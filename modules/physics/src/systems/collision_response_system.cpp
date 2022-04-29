#include <lemon/physics/systems/collision_response_system.h>

#include <lemon/core/math/math.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>
#include <lemon/scene/scene.h>

namespace lemon {
collision_response_system::collision_response_system(ptr<scene> /*s*/, clock& clk, scheduler& sch):
    clk(clk), sch(sch)
{ }
collision_response_system::~collision_response_system()
{ }
void collision_response_system::update(entity_registry& registry)
{
    // TODO: send on collision
    // TODO: fix this somehow
    // 1. Use existing group to speedup lookup?
    // 2. cache results for pairs of rigidbodies?

    auto collisions = registry.view<collision_m>();
    sch.for_each(
        collisions.begin(), collisions.end(),
        [&](auto ent) {
            auto&& [coll]            = collisions.get(ent);
            auto&& [trA, collA, rbA] = registry.get<transform, collider, rigidbody>(coll.A);
            auto&& [trB, collB]      = registry.get<transform, collider>(coll.B);

            // impulse collision response
            auto bounciness = std::min(collA.bounciness, collB.bounciness);
            auto vj         = -(1 + bounciness) * dot(rbA.velocity, coll.mtv.axis);
            auto j          = vj;
            rbA.velocity += j * coll.mtv.axis;
        });

    // TODO: trigger response
    auto triggers = registry.view<trigger_m>();
    sch.for_each(
        triggers.begin(), triggers.end(),
        [&](auto ent) {
            auto&& [trg] = triggers.get(ent);
            (void)trg;
        });
}

} // namespace lemon
