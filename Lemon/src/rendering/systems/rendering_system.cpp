#include "world/components/entity_components.h"
#include <rendering/systems/rendering_system.h>

#include <rendering/rendering_context.h>

#include <platform/window_events.h>

namespace lemon {
rendering_system::rendering_system(
    scene& _scene, event_queue& _eventQueue, asset_storage& _assetStorage, window& _window):
    _scene(_scene),
    renderer(_assetStorage),
    viewport(_window.get_size())
{
    update = _eventQueue["Render"_hs] += [this](event_args* e) {
        this->on_render(e);
    };
    framebufferSize = _eventQueue["FramebufferSize"_hs] += [this](event_args* e) {
        auto&& [width, height] = get_event<framebuffer_size>(e);
        viewport.x             = width;
        viewport.y             = height;
    };
}

rendering_system::~rendering_system()
{ }

void rendering_system::on_render([[maybe_unused]] event_args* e)
{
    mainCamera        = _scene.view<main_camera_t>().front();
    auto&& [cam, mod] = _scene.get<const camera, const model>(mainCamera);
    auto newViewport  = vec4{
        cam.viewport.x * viewport.x,
        cam.viewport.y * viewport.y,
        cam.viewport.z * viewport.x,
        cam.viewport.w * viewport.y
    };
    const f32 aspect     = newViewport.z / newViewport.w;
    const f32 halfHeight = newViewport.w * cam.size * 0.5f;
    const f32 halfWidth  = halfHeight * aspect;

    const auto proj = glm::ortho(
        -halfWidth,
        halfWidth,
        -halfHeight,
        halfHeight, 0.f, 10.f);
    const auto viewProj = proj * glm::inverse(mod.matrix);

    rendering_context::set_viewport(newViewport);
    rendering_context::clear_screen(color{ 0.0f, 0.0f, 0.0f, 1.0f });
    renderer.start_render(viewProj);

    auto group = _scene.group<sprite_renderer, model>(entt::get<enabled_t>);
    group.sort([&](auto entl, auto entr) {
        auto&& [ls, lm] = group.get<sprite_renderer, model>(entl);
        auto&& [rs, rm] = group.get<sprite_renderer, model>(entr);
        return lm.matrix[3].z < rm.matrix[3].z;
    });

    group.each([this](const auto, auto& sr, auto& m) {
        if (sr.tex)
            renderer.render_sprite(sr.col, sr.texCoords, sr.tex, m.matrix);
    });
    renderer.end_render();
}
}
