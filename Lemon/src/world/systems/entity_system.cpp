#include <world/systems/entity_system.h>

#include <world/components/entity_components.h>
#include <world/scene.h>

namespace lemon {
void entity_system::add_to_registry(registry& _registry, entity_t handle)
{
    auto& _tag = _registry.get<tag>(handle);
    _scene.get_entity_registry().push(_tag.name, handle);
}

entity_system::entity_system(scene& _scene, event_queue& eventQueue, message_bus& _messageBus):
    _scene(_scene), _messageBus(_messageBus)
{
    update = eventQueue["EarlyUpdate"_hs] += [this](event_args* e) {
        this->on_early_update(e);
    };

    mount = eventQueue["OnSceneLoaded"] += [this](event_args* e) {
        this->on_mount(e);
    };
}

entity_system::~entity_system()
{
    auto& _reg = _scene.get_registry();
    _reg.on_construct<tag>().disconnect<&entity_system::add_to_registry>(this);
}

void entity_system::on_mount([[maybe_unused]] event_args* e)
{
    auto& entityRegistry = _scene.get_entity_registry();

    for (auto&& [_entity, _tag] : _scene.view<tag>().each())
    {
        entityRegistry.push(_tag.name, _entity);
    }

    auto& _reg = _scene.get_registry();
    _reg.on_construct<tag>().connect<&entity_system::add_to_registry>(this);
}

void entity_system::on_early_update([[maybe_unused]] event_args* e)
{
    auto&& toDestroy = _scene.view<destroy_m>();
    _scene.destroy(toDestroy.begin(), toDestroy.end());

    _scene.view<disable_m>()
        .each([this](const auto handle) {
            entity(_scene, handle)
                .remove_component<enabled_t>();
            _messageBus.push_message(u32(handle), "on_disable");
        });

    _scene.view<enable_m>()
        .each([this](const auto handle) {
            entity(_scene, handle)
                .emplace_or_replace<enabled_t>();
            _messageBus.push_message(u32(handle), "on_enable");
        });
}
}
