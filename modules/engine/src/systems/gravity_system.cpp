#include <lemon/engine/systems/gravity_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/scene/components/physics_components.h>
#include <lemon/scene/scene.h>

namespace lemon {
gravity_system::gravity_system(ptr<scene> s, scheduler& sch, clock& clk):
    gravity(-9.81f), sch(sch), clk(clk)
{
}
gravity_system::~gravity_system()
{
}
void gravity_system::update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    auto deltaTime = clk.delta_time();

    auto view = registry.view<rigidbody>();
    sch.for_each(view.begin(), view.end(),
                 [&](auto ent, auto& rb) {
                     rb.velocity.y += (gravity * rb.gravityScale) * deltaTime;
                 });
}
} // lemon