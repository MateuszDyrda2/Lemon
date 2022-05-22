#include <lemon/physics/systems/prephysics_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>

namespace lemon {
prephysics_system::prephysics_system(service_registry& globalRegistry):
    system(globalRegistry)
{ }
prephysics_system::~prephysics_system()
{ }
void prephysics_system::on_update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();

    auto view = registry.view<dirty_t, rigidbody, transform>();
    for(auto&& [ent, rb, tr] : view.each())
    {
        rb.position = tr.position;
        rb.rotation = tr.rotation;
    }
}
} // namespace lemon
