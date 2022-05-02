#include <lemon/physics/systems/movement_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/core/math/math.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/scene/components/player_components.h>
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

    auto view = registry.view<rigidbody, entity_controller, move_m>();
    sch.for_each(
        view.begin(), view.end(),
        [&](auto ent) {
            auto&& [rb, ec, mm] = view.get(ent);
            const auto vel      = mm.direction * ec.speed;
            rb.velocity         = vel * deltaTime;
        });
}

} // lemon
