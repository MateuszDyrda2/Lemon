#include <lemon/engine/systems/physics_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/scene/components/physics_components.h>
#include <lemon/scene/components/player_components.h>
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

    auto view = registry.view<rigidbody, entity_controller, move_m>();
    sch.for_each(
        view.begin(), view.end(),
        [&](auto /*ent*/, auto& rb, auto& ec, auto& mm) {
            auto vel = mm.direction * ec.speed;
            rb.velocity += vel * deltaTime;
        });
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
