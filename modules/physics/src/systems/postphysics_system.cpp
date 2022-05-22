#include <lemon/physics/systems/postphysics_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>

namespace lemon {
postphysics_system::postphysics_system(service_registry& globalRegistry):
    system(globalRegistry), clk(globalRegistry.get_service<game_clock>())
{ }
postphysics_system::~postphysics_system()
{ }
void postphysics_system::on_update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    auto alpha   = f32(clk.get_alpha());
    auto alphamo = alpha - 1.f;

    auto view = registry.view<transform, rigidbody>();
    for(auto&& [ent, tr, rb] : view.each())
    {
        tr.position = rb.position * alpha + rb._oldPosition * alphamo;
        tr.rotation = rb.rotation * alpha + rb._oldRotation * alphamo;
    }
}
} // namespace lemon
