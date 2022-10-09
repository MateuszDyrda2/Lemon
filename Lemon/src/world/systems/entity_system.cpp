#include <world/systems/entity_system.h>

namespace lemon {
entity_system::entity_system(scene& _scene, event_queue& eventQueue):
    _scene(_scene)
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
    auto&& toDestroy = _scene.view<destroy_m>();
    _scene.destroy(toDestroy.begin(), toDestroy.end());

    _scene.view<disable_m>()
        .each([this](const auto handle) {
            entity(_scene, handle)
                .remove_component<enabled_t>();
        });

    _scene.view<enable_m>()
        .each([this](const auto handle) {
            entity(_scene, handle)
                .emplace_or_replace<enabled_t>();
        });
}
}
