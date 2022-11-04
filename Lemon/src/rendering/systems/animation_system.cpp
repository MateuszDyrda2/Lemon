#include "rendering/components/rendering_components.h"
#include <rendering/systems/animation_system.h>

#include <world/scene.h>

namespace lemon {
animation_system::animation_system(scene& _scene, event_queue& _eventQueue):
    _scene(_scene)
{
    update = _eventQueue["Render"_hs] += [this](event_args* e) {
        this->on_render(e);
    };
}

animation_system::~animation_system() { }

void animation_system::on_render([[maybe_unused]] event_args* e) noexcept
{
    auto&& [delta, _] = get_event<update_event>(e);

    auto start = _scene.view<start_animation_m>();
    for (auto&& [_entity, _sa] : start.each())
    {
        _scene.get_entity(_entity).emplace_or_replace<animating_t>(_sa.name);
    }
    _scene.clear<start_animation_m>();

    auto end = _scene.view<stop_animation_m>();
    for (auto&& [_entity] : end.each())
    {
        _scene.get_entity(_entity).remove_component<animating_t>();
    }
    _scene.clear<stop_animation_m>();

    std::vector<entity_t> toRemove;
    auto group = _scene.group<animation_component, animating_t>(entt::get<sprite_renderer>);
    for (auto&& [_entity, _animationComponent, _animating, _sr] : group.each())
    {
        auto&& [nFrames, frameSize, animation] = _animationComponent;
        auto res                               = animation.get()->execute(_animating.animationName, _animating.duration);
        if (res < 0)
        {
            toRemove.push_back(_entity);
            continue;
        }
        auto tex            = _sr.tex.get();
        const auto& texSize = tex->get_size();
        auto w              = texSize.x / frameSize.x;
        auto c              = res / w;
        auto r              = res % w;
        vec4 texOffset      = vec4{ f32(r) * frameSize.x / texSize.x, f32(c) * frameSize.y / texSize.y,
                               f32(r + 1) * frameSize.x / texSize.x, f32(c + 1) * frameSize.y / texSize.y };

        _sr.texCoords = texOffset;
        _animating.duration += delta;
    }

    for (const auto ent : toRemove)
    {
        _scene.get_entity(ent).remove_component<animating_t>();
    }
}
}
