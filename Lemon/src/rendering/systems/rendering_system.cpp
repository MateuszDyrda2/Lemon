#include <rendering/systems/rendering_system.h>

#include <rendering/rendering_context.h>

#include <platform/window_events.h>

namespace lemon {
rendering_system::rendering_system(
    registry& _registry, event_queue& _eventQueue, asset_storage& _assetStorage, window& _window):
    _registry(_registry),
    renderer(_assetStorage),
    viewport(_window.get_size())
{
    framebufferSize = _eventQueue["FramebufferSize"_hs] += [this](event_args* e) {
        auto&& [width, height] = get_event<framebuffer_size>(e);
        viewport.x             = width;
        viewport.y             = height;
    };
}

rendering_system::~rendering_system()
{ }

void rendering_system::update()
{
    mainCamera        = _registry.view<main_camera_t>().front();
    auto&& [cam, mod] = _registry.get<camera, model>(mainCamera);
    auto newViewport  = vec4{
        cam.viewport.x * viewport.x,
        cam.viewport.y * viewport.y,
        cam.viewport.z * viewport.x,
        cam.viewport.w * viewport.y
    };
    const auto res = newViewport.z / newViewport.w;
    const auto w   = res * cam.size * 0.5f;
    const auto h   = cam.size * 0.5f;
    auto proj      = glm::ortho((-1.0f) * w, w, (-1.0f) * h, h);
    auto modelMat  = mod.matrix;
    auto viewProj  = proj * modelMat;

    rendering_context::set_viewport(newViewport);
    rendering_context::clear_screen(color{ 0.5f, 0.5f, 0.5f, 1.0f });
    renderer.start_render(viewProj);

    _registry.view<sprite_renderer, model>().each(
        [this](const auto, auto& sr, auto& m) {
            if(sr.tex)
                renderer.render_sprite(sr.col, sr.texCoords, sr.tex, m.matrix);
        });
    renderer.end_render();
}
}
