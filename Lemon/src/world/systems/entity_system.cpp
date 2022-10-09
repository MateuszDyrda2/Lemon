#include <world/systems/entity_system.h>

namespace lemon {
entity_system::entity_system(scene& _scene, event_queue& eventQueue):
    _registry(_scene.get_registry())
{
    update = eventQueue["EarlyUpdate"_hs] += [this](event_args* e) {
        this->onEarlyUpdate(e);
    };
}

entity_system::~entity_system()
{
}

void entity_system::onEarlyUpdate([[maybe_unused]] event_args* e)
{
    auto&& toDestroy = _registry.view<destroy_m>();
    _registry.destroy(toDestroy.begin(), toDestroy.end());

    _registry.view<disable_m>()
        .each([this](const auto entity) {
            _registry.remove<enabled_t>(entity);
        });

    _registry.view<enable_m>()
        .each([this](const auto entity) {
            _registry.emplace_or_replace<enabled_t>(entity);
        });
}
}
