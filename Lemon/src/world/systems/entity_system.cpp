#include <world/systems/entity_system.h>

namespace lemon {
entity_system::entity_system(registry& _registry):
    _registry(_registry) { }

entity_system::~entity_system()
{
}

void entity_system::update()
{
    auto&& toDestroy = _registry.view<destroy_m>();
    _registry.destroy(toDestroy.begin(), toDestroy.end());

    _registry.view<disable_m>()
        .each([this](const entity entity) {
            _registry.remove<enabled_t>(entity);
        });

    _registry.view<enable_m>()
        .each([this](const entity entity) {
            _registry.emplace_or_replace<enabled_t>(entity);
        });
}
}
