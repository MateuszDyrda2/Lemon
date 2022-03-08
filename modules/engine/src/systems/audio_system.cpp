#include <lemon/engine/systems/audio_system.h>

#include <lemon/scene/scene.h>

namespace lemon {
audio_system::audio_system(ptr<scene> s):
    player(), listener(s->get_main_camera())
{
}
audio_system::~audio_system()
{
}
void audio_system::update(entity_registry& registry)
{
    auto [listenerComponent, listenerTransform] =
        listener.get_component<audio_listener, transform>();

    player.start_playing(
        listenerComponent.masterGain, listenerTransform.position);
    auto group = registry.group<audio_source, play>(entt::get<transform>);
    for(auto entity : group)
    {
        auto& aComponent = group.get<audio_source>(entity);
        auto& tComponent = group.get<transform>(entity);
        player.play(aComponent.pitch, aComponent.gain, aComponent.loop,
                    *aComponent.clip.get(), aComponent.source,
                    tComponent.position);
    }
    registry.clear<play>();
}
void audio_system::begin_play(entity ent)
{
    LEMON_ASSERT((ent.has_all_of<audio_source, transform>()));
    auto&& aComponent = ent.get_component<audio_source>();
    LEMON_ASSERT(aComponent.clip);
    ent.add_component<play>();
}
void audio_system::begin_play(entity ent, const asset<sound>& buffer)
{
    LEMON_ASSERT((ent.has_all_of<audio_source, transform>()));
    auto&& aComponent = ent.get_component<audio_source>();
    aComponent.clip   = buffer;
    ent.add_component<play>();
}

} // namespace lemon
