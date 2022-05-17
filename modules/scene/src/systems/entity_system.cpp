#include <lemon/scene/systems/entity_system.h>

#include <lemon/scene/components/entity_components.h>

namespace lemon {
entity_system::entity_system(service_registry& globalRegistry):
    system(globalRegistry)
{ }
entity_system::~entity_system()
{ }
void entity_system::on_update(entity_registry& registry)
{
    // cleanup destroyed entities
    auto&& toDestroy = registry.view<destroy_m>();
    registry.destroy(toDestroy.begin(), toDestroy.end());

    auto&& toEnable = registry.view<enable_m>();
    toEnable.each([&](auto ent) {
        registry.emplace_or_replace<enabled_t>(ent);
    });

    auto&& toDisable = registry.view<disable_m>();
    toDisable.each([&](auto ent) {
        registry.remove<enabled_t>(ent);
    });
}

} // namespace lemon
