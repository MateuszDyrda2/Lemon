#include <lemon/audio/audio_system.h>

#include <lemon/game/scene.h>

namespace lemon {
audio_system::audio_system(ptr<scene> s)
{
}
audio_system::~audio_system()
{
}
void audio_system::update(entity_registry& registry)
{
    auto [listenerComponent, listenerTransform] =
        listener.get_component<audio_listener, transform>();

    player.start_playing(listenerComponent, listenerTransform);
    auto group = registry.group<audio_source, play>(entt::get<transform>);
    for(auto entity : group)
    {
        auto& aComponent = group.get<audio_source>(entity);
        auto& tComponent = group.get<transform>(entity);
        player.play(aComponent, tComponent);
    }
    registry.clear<play>();
}
} // namespace lemon
